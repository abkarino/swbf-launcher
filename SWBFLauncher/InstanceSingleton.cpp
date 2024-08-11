#include "pch.h"
#include "InstanceSingleton.h"

bool InstanceSingleton::initialized = false;
PVOID InstanceSingleton::base = NULL;
PROCESS_INFORMATION InstanceSingleton::processInformation;