#include "stdafx.h"
#include "resource.h"
#include "PriorityMap.h"

const UINT g_PriorityResourceStringMap[] = {
	IDS_HIGH_PRIORITY_CLASS,
	IDS_ABOVE_NORMAL_PRIORITY_CLASS,
	IDS_NORMAL_PRIORITY_CLASS,
	IDS_BELOW_NORMAL_PRIORITY_CLASS,
	IDS_IDLE_PRIORITY_CLASS
};

const DWORD g_PriorityClassMap[] = {
	HIGH_PRIORITY_CLASS,
	ABOVE_NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	BELOW_NORMAL_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS
};

int PriorityClassToIndex(DWORD priorityClass)
{
	int priorityIndex = 2;
	for(int i = 0; i < 5; ++i){
		if(g_PriorityClassMap[i] == priorityClass){
			priorityIndex = i;
			break;
		}
	}
	return priorityIndex;
}