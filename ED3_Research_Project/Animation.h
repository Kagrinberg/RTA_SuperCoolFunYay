#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <vector>


// This stores the information of each key frame of each joint
// This is a linked list and each node is a snapshot of the
// global transformation of the joint at a certain frame
struct Keyframe
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	Keyframe* mNext;

	Keyframe() : mNext(nullptr)
	{

	}
};

// This is the actual representation of a joint in a game engine
struct Joint
{
	std::string mName;
	int mParentIndex;
	FbxAMatrix mGlobalBindposeInverse;
	Keyframe* mAnimation;
	FbxNode* mNode;

	Joint() : mNode(nullptr), mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		while (mAnimation)
		{
			Keyframe* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}
};

struct Skeleton
{
	std::vector<Joint> mJoints;
};

class Animation
{
	FbxScene* mFBXScene;
	bool mHasAnimation;
	Skeleton mSkeleton;
	FbxLongLong mAnimationLength;
	std::string mAnimationName;

public:
	Animation();
	~Animation();
	void CreateSkeleton();
	void SkeleRecursive(FbxNode * currentNode, int curIndex, int parentIndex);
	void setScene(FbxScene* inFBXScene);
	unsigned int getJoint(std::string jName);

};