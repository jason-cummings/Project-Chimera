#include "GameState.hpp"

GameState::GameState():
    render_system( RenderSystem::getRenderSystem() ),
    init_success( true ),
    quit_game( false ),
    should_destroy_on_state_change( false ),
    mouse_lock( false ),
    scene( nullptr ),
    next_state( nullptr ) {
    
}

GameState::~GameState() {}

/** 
 * Set the state engine should switch to next
 * @param input - state that Engine should swap to
 * @param destroy_self - should Engine destroy this state after swapping to input
 */
void GameState::setNextState( GameState * input, bool destroy_self ) {
    next_state = input;
    should_destroy_on_state_change = destroy_self;
}

GameState* GameState::getNextState() {
    GameState *ret = next_state;
    next_state = nullptr;
    return ret;
}

void GameState::handleSDLEvent( SDL_Event e ) {
    switch( e.type ) {
        case SDL_KEYDOWN:           handleKeyDown( e );         break;
        case SDL_KEYUP:             handleKeyUp( e );           break;
        case SDL_MOUSEMOTION:       handleMouseMotion( e );     break;
        case SDL_MOUSEBUTTONDOWN:   handleMouseButtonDown( e ); break;
        case SDL_MOUSEBUTTONUP:     handleMouseButtonUp( e );   break;
        case SDL_MOUSEWHEEL:        handleMouseWheel( e );      break;
    }
}

void GameState::handleKeyDown( SDL_Event e ) {
    SDL_Keycode key = e.key.keysym.sym;
    if( key == SDLK_F1 ) {
        // Cycle shadow modes
        UserSettings::shadow_mode = static_cast<ShadowMode>((static_cast<int>(UserSettings::shadow_mode) + 1) % 4);
    }
    else if( key == SDLK_F2 ) {
        // Cycle bloom modes
        UserSettings::bloom_mode = static_cast<BloomMode>((static_cast<int>(UserSettings::bloom_mode) + 1) % 3);
    }
    else if( key == SDLK_F4 ) {
        if( UserSettings::resolution_width > 2000 ) {
            UserSettings::resolution_width = 640;
            UserSettings::resolution_height = 480;
        }
        else {
            UserSettings::resolution_width = 2560;
            UserSettings::resolution_height = 1440;
        }
        render_system.recreateFramebuffers();
    }
    else if( key == SDLK_F5 ) {
        UserSettings::use_volumetric_light_scattering = !UserSettings::use_volumetric_light_scattering;
    }
    
    handleKeyDownStateSpecific(e);
}

void GameState::handleKeyUp( SDL_Event e ) {
    handleKeyUpStateSpecific(e);
}

void GameState::handleMouseMotion( SDL_Event e ) {
    handleMouseMotionStateSpecific(e);
}

void GameState::handleMouseButtonDown( SDL_Event e ) {
    handleMouseButtonDownStateSpecific(e);
}

void GameState::handleMouseButtonUp( SDL_Event e ) {
    handleMouseButtonUpStateSpecific(e);
}

void GameState::handleMouseWheel( SDL_Event e ) {
    handleMouseWheelStateSpecific(e);
}
