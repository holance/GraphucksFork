#pragma once
#include ".\Rendering\RenderContext.h"

#include ".\Core\SceneRoot.h"
#include ".\Core\CompositeTransform.h"
#include ".\Core\MatrixTransform.h"

#include ".\Effects\DropShadowEffect.h"
#include ".\Effects\BlurEffect.h"
#include ".\Effects\SaturationEffect.h"

#include ".\Brushes\ImageBrush.h"
#include ".\Brushes\SolidColorBrush.h"
#include ".\Brushes\RadialGradientBrush.h"
#include ".\Brushes\LinearGradientBrush.h"
#include ".\Core\WinRT\XamlSceneRoot.h"
#include ".\Core\Win32\Win32SceneRoot.h"

#include ".\UI\ContentControlElement.h"
#include ".\UI\RectangleElement.h"
#include ".\UI\StackPanel.h"
#include ".\UI\MultiContentPanelElement.h"
#include ".\UI\Canvas.h"
#include ".\UI\RadialPanel.h"
#include ".\UI\EllipseElement.h"
#include ".\UI\PathElement.h"
#include ".\UI\TextBlockElement.h"
#include ".\UI\BorderElement.h"
#include ".\UI\ImageElement.h"
#include ".\UI\MediaElement.h"
#include ".\Media\MediaPlayer.h"

#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "shcore.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "GraphucksLib.lib")
