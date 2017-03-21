#ifndef ECS_H
#define ECS_H

#include "tgEntitySystemManager.h"
#include "tgEntity.h"
#include "tgComponent.h"
#include "tgSystem.h"

#include "tgComponentHandler.h"

template <typename T>
using tgHandler = tgComponentHandler<T>;

#endif