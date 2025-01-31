#include "gameobject.h"

void GameObject::playAnimation() {
	if (!currentModel)
		return;

	playingAnimation = true;
}

void GameObject::stopAnimation() {
	if (!currentModel)
		return;

	playingAnimation = false;
}