#include "AnimationFactory.hpp"

AnimationCurve * AnimationFactory::getTranslationAnimationCurve(fs::path dir, GameObject * scenegraph) {
	TranslationAnimationCurve * curve = new TranslationAnimationCurve();

	fs::path keyframe_path = dir;
	keyframe_path.append("translation");
	Asset keyframes_asset(keyframe_path);
	if(keyframes_asset.getBuffer() == nullptr) {
		return nullptr;
	}


	VecKeyframe * buffer = (VecKeyframe *)keyframes_asset.getBuffer();

	int num_keyframes = keyframes_asset.getBytes() / sizeof(VecKeyframe);

	for(int i = 0; i < num_keyframes; i++) {
		curve->addKeyframe(buffer[i]);
	}

	curve->setTarget(scenegraph->getGameObject(dir.filename().string()));

	// std::cout << "Creating Translation Curve targetting " << curve->getTarget()->getID() << std::endl;

	return (AnimationCurve *)curve;
}

AnimationCurve * AnimationFactory::getRotationAnimationCurve(fs::path dir, GameObject * scenegraph) {
	RotationAnimationCurve * curve = new RotationAnimationCurve();

	fs::path keyframe_path = dir;
	keyframe_path.append("rotation");
	Asset keyframes_asset(keyframe_path);
	if(keyframes_asset.getBuffer() == nullptr) {
		return nullptr;
	}


	VecKeyframe * buffer = (VecKeyframe *)keyframes_asset.getBuffer();

	int num_keyframes = keyframes_asset.getBytes() / sizeof(VecKeyframe);

	for(int i = 0; i < num_keyframes; i++) {
		buffer[i].value[0] = glm::radians(buffer[i].value[0]);
		buffer[i].value[1] = glm::radians(buffer[i].value[1]);
		buffer[i].value[2] = glm::radians(buffer[i].value[2]);
		curve->addKeyframe(QuatKeyframe(buffer[i]));
	}

	curve->setTarget(scenegraph->getGameObject(dir.filename().string()));

	// std::cout << "Creating Rotation Curve targetting " << curve->getTarget()->getID() << std::endl;

	return (AnimationCurve *)curve;
}


AnimationLayer * AnimationFactory::getAnimationLayer(fs::path dir, GameObject * scenegraph) {
	std::string name = dir.filename().string();
	AnimationLayer * layer = new AnimationLayer(name);

	for( auto& curve_dir: fs::directory_iterator(dir) ) {
		if(fswrapper::is_dir(curve_dir)) {
	        fs::path curve_path = curve_dir.path();
	        AnimationCurve * translate_curve = getTranslationAnimationCurve(curve_path,scenegraph);
	        if(translate_curve != nullptr)
	        	layer->addCurve(translate_curve);

	        AnimationCurve * rotate_curve = getRotationAnimationCurve(curve_path,scenegraph);
	        if(rotate_curve != nullptr)
	        	layer->addCurve(rotate_curve);
	    }
    }

    return layer;
}



AnimationStack * AnimationFactory::getAnimationStack(fs::path dir, GameObject * scenegraph){
	std::string name = dir.filename().string();
	AnimationStack * stack = new AnimationStack(name);

	for( auto& layer_dir: fs::directory_iterator(dir) ) {
		if(fswrapper::is_dir(layer_dir)) {
	        fs::path layer_path = layer_dir.path();
	        AnimationLayer * layer = getAnimationLayer(layer_path,scenegraph);
	        stack->addAnimationLayer(layer);
	    }
    }

    return stack;
}