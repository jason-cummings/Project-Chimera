#ifndef ANIMATION_FACTORY_H
#define ANIMATION_FACTORY_H

#include "Keyframe.hpp"
#include "AnimationCurve.hpp"
#include "AnimationLayer.hpp"
#include "AnimationStack.hpp"
#include "../../Utilities/FilesystemWrapper.hpp"

class AnimationFactory {
	static AnimationCurve * getTranslationAnimationCurve(fs::path dir, GameObject * scenegraph);
	static AnimationCurve * getRotationAnimationCurve(fs::path dir, GameObject * scenegraph);


	static AnimationLayer * getAnimationLayer(fs::path dir, GameObject * scenegraph);

public:
	static AnimationStack * getAnimationStack(fs::path dir, GameObject * scenegraph);
};

#endif