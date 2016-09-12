#include "Animation.h"


Animation::Animation() : mAnimationName("NoName")
{

}


Animation::~Animation()
{
	for (auto it = mControlPoints.begin(); it != mControlPoints.end(); it++) {
		delete it->second;
	}
}

void Animation::setScene(FbxScene* inFBXScene)
{
	mFBXScene = inFBXScene;
}

int Animation::getJoint(std::string jName)
{
	for (unsigned int i = 0; i < mSkeleton.mJoints.size(); i++)
	{
		if (mSkeleton.mJoints[i].mName == jName)
		{
			return i;
		}
	}

	return -1;
}

bool Animation::isAnimated()
{
	return mHasAnimation;
}

void Animation::CreateSkeleton()
{
	FbxNode * rootNode =  mFBXScene->GetRootNode();

	for (int cIndex = 0; cIndex < rootNode->GetChildCount(); cIndex++)
	{
		FbxNode * currentNode = rootNode->GetChild(cIndex);
		SkeleRecursive(currentNode, 0, -1);
	}

	if (mSkeleton.mJoints.empty())
	{
		mHasAnimation = false;
	}
	else
	{
		mHasAnimation = true;
	}

}

void Animation::SkeleRecursive(FbxNode * currentNode, int curIndex, int parentIndex)
{
	if (currentNode->GetNodeAttribute() && currentNode->GetNodeAttribute()->GetAttributeType() && currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint myJoint;
		myJoint.mParentIndex = parentIndex;
		myJoint.mName = currentNode->GetName();
		mSkeleton.mJoints.push_back(myJoint);
	}
	if (currentNode->GetNodeAttribute() && currentNode->GetNodeAttribute()->GetAttributeType() && currentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		meshNode = currentNode;
	}
	for (int i = 0; i < currentNode->GetChildCount(); i++)
	{
		SkeleRecursive(currentNode->GetChild(i), mSkeleton.mJoints.size(), curIndex);
	}
}

//void Animation::setMesh(Mesh * theMesh)
//{
//	myMesh = theMesh;
//}

void Animation::makeCpts()
{
	unsigned int cptsTotal = meshNode->GetMesh()->GetControlPointsCount();
	for (unsigned int i = 0; i < cptsTotal; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();
		currCtrlPoint->controlIndex = i;
		mControlPoints[i] = currCtrlPoint;
	}
}

void Animation::checkControls()
{
	unsigned int cptsTotal = meshNode->GetMesh()->GetControlPointsCount();
	for (unsigned int i = 0; i < cptsTotal; ++i)
	{
		if (mControlPoints[i]->jointIndex.size() < 4)
		{
			int toDo = 4 - (mControlPoints[i]->jointIndex.size());
			for (toDo; toDo > 0; toDo--)
			{
				mControlPoints[i]->jointIndex.push_back(0);
				mControlPoints[i]->jointWeights.push_back(0.0f);
			}
		}
	}
}

void Animation::createWeights()
{
	FbxMesh * curMesh = meshNode->GetMesh();
	unsigned int totalDeforms = curMesh->GetDeformerCount();

	FbxVector4 lT = meshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 lR = meshNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 lS = meshNode->GetGeometricScaling(FbxNode::eSourcePivot);

	//will probably be identity, this is created just in case not using maya or random akwardness has occured
	FbxAMatrix geometeryTransform = FbxAMatrix(lT,lR,lS);

	for (unsigned int deformerNum = 0; deformerNum < totalDeforms; deformerNum++)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(curMesh->GetDeformer(deformerNum, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int totalClusters = currSkin->GetClusterCount();
		for (unsigned int clusterNum = 0; clusterNum < totalClusters; clusterNum++)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterNum);
			std::string currJointName = currCluster->GetLink()->GetName();
			int currJointIndex = getJoint(currJointName);

			if (currJointIndex == -1)
			{
				continue;
			}

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometeryTransform;

			// Update the information in mSkeleton 
			mSkeleton.mJoints[currJointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			mSkeleton.mJoints[currJointIndex].mNode = currCluster->GetLink();

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				mControlPoints[currCluster->GetControlPointIndices()[i]]->jointIndex.push_back(currJointIndex);
				mControlPoints[currCluster->GetControlPointIndices()[i]]->jointWeights.push_back(static_cast<float>(currCluster->GetControlPointWeights()[i]));
			}


			//getanimation information
			FbxAnimStack* theAniStack = mFBXScene->GetSrcObject<FbxAnimStack>(0);

			FbxString animStackName = theAniStack->GetName();
			mAnimationName = animStackName.Buffer();

			FbxTakeInfo* takeInfo = mFBXScene->GetTakeInfo(animStackName);

			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames24);
				Keyframe * currAnim = new Keyframe();
				currAnim->mFrameNum = i;

				FbxAMatrix currentTransformOffset = meshNode->EvaluateGlobalTransform(currTime) * geometeryTransform;

				currAnim->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

				mSkeleton.mJoints[currJointIndex].mAnimation.push_back(currAnim);
			}
			
		}



	}
}

Skeleton Animation::getSkele()
{
	return mSkeleton;
}

std::unordered_map<unsigned int, CtrlPoint*>& Animation::getMap()
{
	return mControlPoints;
}

int Animation::getAniLength()
{
	return static_cast<int>(mAnimationLength);
}