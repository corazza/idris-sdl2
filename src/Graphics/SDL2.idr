module Graphics.SDL2

import Graphics.Config

%include C "sdl2.h"
%link C    "sdl2.o"

-- Set up a window

export
data Renderer = MkRenderer Ptr

export
data Surface = MkSurface Ptr

export
data Texture = MkTexture Ptr

export
init : Int -> Int -> IO Renderer
init x y = do
  ptr <- foreign FFI_C "idris_sdl2_init" (Int -> Int -> IO Ptr) x y
  pure $ MkRenderer ptr

export
setRendererDrawColor :
  Renderer ->
  (r : Bits8) -> (g : Bits8) -> (b : Bits8) -> (a : Bits8) -> IO Bool
setRendererDrawColor (MkRenderer renderer) r g b a = do
  result <- foreign FFI_C "SDL_SetRenderDrawColor"
            (Ptr -> Bits8 -> Bits8 -> Bits8 -> Bits8 -> IO Int)
            renderer r g b a
  pure (result == 0)

export
getError : IO String
getError = foreign FFI_C "SDL_GetError" (IO String)

export
quit : IO ()
quit = foreign FFI_C "SDL_Quit" (IO ())

-- XXX: Ignores last two arguments for now: srcRect and dstRect.
export
renderCopy : Renderer -> Texture -> IO Int
renderCopy (MkRenderer renderer) (MkTexture texture) =
  foreign FFI_C "SDL_RenderCopy" (Ptr -> Ptr -> Ptr -> Ptr -> IO Int)
          renderer texture null null

export
renderClear : Renderer -> IO Int
renderClear (MkRenderer renderer) =
    foreign FFI_C "SDL_RenderClear" (Ptr -> IO Int) renderer

export
renderPresent : Renderer -> IO ()
renderPresent (MkRenderer ptr)
     = foreign FFI_C "SDL_RenderPresent" (Ptr -> IO ()) ptr

export
loadBMP : (bmpPath : String) -> IO Surface
loadBMP bmpPath = do
  surface <- foreign FFI_C "SDL_LoadBMP" (String -> IO Ptr) bmpPath
  pure $ MkSurface surface

export
createTextureFromSurface : Renderer -> (bitmap : Surface) -> IO Texture
createTextureFromSurface (MkRenderer renderer) (MkSurface bmp) = do
  texture <- foreign FFI_C "SDL_CreateTextureFromSurface" (Ptr -> Ptr -> IO Ptr) renderer bmp
  pure $ MkTexture texture

export
destroyRenderer : Renderer -> IO ()
destroyRenderer (MkRenderer renderer) =
  foreign FFI_C "SDL_DestroyRenderer" (Ptr -> IO ()) renderer

export
freeSurface : Surface -> IO ()
freeSurface (MkSurface surface) =
  foreign FFI_C "SDL_FreeSurface" (Ptr -> IO ()) surface

-- Some drawing primitives

export
filledRect : Renderer -> Int -> Int -> Int -> Int ->
                           Int -> Int -> Int -> Int -> IO ()
filledRect (MkRenderer ptr) x y w h r g b a
      = foreign FFI_C "filledRect"
           (Ptr -> Int -> Int -> Int -> Int ->
            Int -> Int -> Int -> Int -> IO ()) ptr x y w h r g b a

export
filledEllipse : Renderer -> Int -> Int -> Int -> Int ->
                              Int -> Int -> Int -> Int -> IO ()
filledEllipse (MkRenderer ptr) x y rx ry r g b a
      = foreign FFI_C "filledEllipse"
           (Ptr -> Int -> Int -> Int -> Int ->
            Int -> Int -> Int -> Int -> IO ()) ptr x y rx ry r g b a

export
drawLine : Renderer -> Int -> Int -> Int -> Int ->
                         Int -> Int -> Int -> Int -> IO ()
drawLine (MkRenderer ptr) x y ex ey r g b a
      = foreign FFI_C "drawLine"
           (Ptr -> Int -> Int -> Int -> Int ->
            Int -> Int -> Int -> Int -> IO ()) ptr x y ex ey r g b a

-- TODO: More keys still to add... careful to do the right mappings in
-- KEY in sdl2.c

public export
data Key = KeyUpArrow
         | KeyDownArrow
         | KeyLeftArrow
         | KeyRightArrow
         | KeyEsc
         | KeySpace
         | KeyTab
         | KeyF1
         | KeyF2
         | KeyF3
         | KeyF4
         | KeyF5
         | KeyF6
         | KeyF7
         | KeyF8
         | KeyF9
         | KeyF10
         | KeyF11
         | KeyF12
         | KeyF13
         | KeyF14
         | KeyF15
         | KeyLShift
         | KeyRShift
         | KeyLCtrl
         | KeyRCtrl
         | KeyAny Char

Eq Key where
  KeyUpArrow    == KeyUpArrow     = True
  KeyDownArrow  == KeyDownArrow   = True
  KeyLeftArrow  == KeyLeftArrow   = True
  KeyRightArrow == KeyRightArrow  = True

  KeyEsc   == KeyEsc   = True
  KeyTab   == KeyTab   = True
  KeySpace == KeySpace = True

  KeyF1    == KeyF1    = True
  KeyF2    == KeyF2    = True
  KeyF3    == KeyF3    = True
  KeyF4    == KeyF4    = True
  KeyF5    == KeyF5    = True
  KeyF6    == KeyF6    = True
  KeyF7    == KeyF7    = True
  KeyF8    == KeyF8    = True
  KeyF9    == KeyF9    = True
  KeyF10   == KeyF10   = True
  KeyF11   == KeyF11   = True
  KeyF12   == KeyF12   = True
  KeyF13   == KeyF13   = True
  KeyF14   == KeyF14   = True
  KeyF15   == KeyF15   = True

  KeyLShift == KeyLShift = True
  KeyRShift == KeyRShift = True
  KeyLCtrl  == KeyLCtrl  = True
  KeyRCtrl  == KeyRCtrl  = True

  (KeyAny x)    == (KeyAny y)     = x == y
  _             == _              = False

public export
data Button = Left | Middle | Right | WheelUp | WheelDown

Eq Button where
  Left == Left = True
  Middle == Middle = True
  Right == Right = True
  WheelUp == WheelUp = True
  WheelDown == WheelDown = True
  _ == _ = False

public export
data Event = KeyDown Key
           | KeyUp Key
           | MouseMotion Int Int Int Int
           | MouseButtonDown Button Int Int
           | MouseButtonUp Button Int Int
           | Resize Int Int
           | AppQuit

Eq Event where
  (KeyDown x) == (KeyDown y) = x == y
  (KeyUp x)   == (KeyUp y)   = x == y
  AppQuit     == AppQuit     = True
  (MouseMotion x y rx ry) == (MouseMotion x' y' rx' ry')
      = x == x' && y == y' && rx == rx' && ry == ry'
  (MouseButtonDown b x y) == (MouseButtonDown b' x' y')
      = b == b' && x == x' && y == y'
  (MouseButtonUp b x y) == (MouseButtonUp b' x' y')
      = b == b' && x == x' && y == y'
  _           == _           = False

export
pollEvent : IO (Maybe Event)
pollEvent = do
  vm <- getMyVM
  MkRaw e <- foreign FFI_C "pollEvent" (Ptr -> IO (Raw (Maybe Event))) vm
  pure e

export
pollEventsForQuit : IO Bool
pollEventsForQuit = do
  quit <- foreign FFI_C "pollEventsForQuit" (IO Int)
  pure $ quit == 1
