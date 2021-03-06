import ctypes, sys, os

from bindings import define
from bindings import CppBindingGenerator as cbg

os.chdir(os.path.dirname(__file__))

if __name__ == '__main__':
    

    # generate
    sharedObjectGenerator = cbg.SharedObjectGenerator(define)

    sharedObjectGenerator.header = '''
#include "Core.h"
#include "BaseObject.h"

#include "Common/Array.h"
#include "Common/ResourceContainer.h"
#include "Common/Resource.h"
#include "Common/Resources.h"

#include "Window/Window.h"

#include "Input/ButtonState.h"
#include "Input/Joystick.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Graphics/Graphics.h"
#include "Graphics/CommandList.h"
#include "Graphics/Texture2D.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/Font.h"
#include "Graphics/ImageFont.h"
#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Renderer/RenderedSprite.h"
#include "Graphics/Renderer/RenderedText.h"
#include "Graphics/Renderer/RenderedPolygon.h"
#include "Graphics/Renderer/RenderedCamera.h"
#include "Graphics/BuiltinShader.h"

#include "Tool/Tool.h"

#include "IO/File.h"
#include "IO/PackFile.h"
#include "IO/StaticFile.h"
#include "IO/StreamFile.h"
#include "IO/FileRoot.h"
#include "IO/BaseFileReader.h"
#include "IO/PackFileReader.h"

#include "Physics/Collider/Collider.h"
#include "Physics/Collider/CircleCollider.h"
#include "Physics/Collider/RectangleCollider.h"
#include "Physics/Collider/PolygonCollider.h"

#include "Sound/Sound.h"
#include "Sound/SoundMixer.h"

#include "Logger/Log.h"

    '''

    sharedObjectGenerator.func_name_create_and_add_shared_ptr = 'Altseed::CreateAndAddSharedPtr'
    sharedObjectGenerator.func_name_add_and_get_shared_ptr = 'Altseed::AddAndGetSharedPtr'

    sharedObjectGenerator.output_path = '../core/src/AutoGeneratedCoreWrapper.cpp'
    sharedObjectGenerator.generate()