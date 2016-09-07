#include "Animation.h"


Animation::Animation() : mAnimationName("NoName")
{

}


Animation::~Animation()
{
}

void Animation::setScene(FbxScene* inFBXScene)
{
	mFBXScene = inFBXScene;
}

unsigned int Animation::getJoint(std::string jName)
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
	for (int i = 0; i < currentNode->GetChildCount(); i++)
	{
		SkeleRecursive(currentNode->GetChild(i), mSkeleton.mJoints.size(), curIndex);
	}
}