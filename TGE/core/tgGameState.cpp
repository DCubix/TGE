#include "tgGameState.h"

#include "tgEngine.h"

tgECS* tgGameState::getECS() { return getEngine()->getECS(); }
