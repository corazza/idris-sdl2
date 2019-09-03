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

-- SDLRects would be added here, but managing those is hard to justify

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

public export
data SDLRect = MkSDLRect Int Int Int Int
%name SDLRect rect

export
translate : Int -> Int -> SDLRect -> SDLRect
translate x' y' (MkSDLRect x y w h) = MkSDLRect (x+x') (y+y') w h

export
drawWholeCenter : (renderer : Renderer) -> (texture : Texture) ->
                  (dst : SDLRect) -> (angle : Double) -> (flip : Int) -> IO ()
drawWholeCenter (MkRenderer renderer) (MkTexture texture) (MkSDLRect x y w h) angle flip
  = foreign FFI_C "drawWholeCenter" (Ptr -> Ptr -> Int -> Int -> Int -> Int -> Int -> Double -> IO ())
            renderer texture x y w h flip angle

export
drawCenter : (renderer : Renderer) -> (texture : Texture) ->
             (src : SDLRect) -> (dst : SDLRect) ->
             (angle : Double) -> (flip : Int) -> IO ()
drawCenter (MkRenderer renderer) (MkTexture texture)
           (MkSDLRect sx sy sw sh) (MkSDLRect dx dy dw dh)
           angle flip
  = foreign FFI_C "drawCenter"
      (Ptr -> Ptr -> Int -> Int -> Int -> Int -> Int -> Int -> Int -> Int -> Int -> Double -> IO ())
      renderer texture sx sy sw sh dx dy dw dh flip angle

export
renderCopy' : (renderer : Renderer) ->
              (texture : Texture) ->
              (src : Maybe SDLRect) ->
              (dst : Maybe SDLRect) -> IO Int
renderCopy' (MkRenderer renderer) (MkTexture texture)
            (Just (MkSDLRect sx sy sw sh)) (Just (MkSDLRect dx dy dw dh))
  = foreign FFI_C "GAME_drawTexture"
            (Ptr -> Ptr ->
             Int -> Int -> Int -> Int ->
             Int -> Int -> Int -> Int ->
             Int -> IO Int)
            renderer texture sx sy sw sh dx dy dw dh 3

renderCopy' (MkRenderer renderer) (MkTexture texture)
            (Just (MkSDLRect sx sy sw sh)) Nothing
  = foreign FFI_C "GAME_drawTexture"
            (Ptr -> Ptr ->
             Int -> Int -> Int -> Int ->
             Int -> Int -> Int -> Int ->
             Int -> IO Int)
            renderer texture sx sy sw sh 0 0 0 0 1

renderCopy' (MkRenderer renderer) (MkTexture texture)
            Nothing (Just (MkSDLRect dx dy dw dh))
  = foreign FFI_C "GAME_drawTexture"
            (Ptr -> Ptr ->
             Int -> Int -> Int -> Int ->
             Int -> Int -> Int -> Int ->
             Int -> IO Int)
            renderer texture 0 0 0 0 dx dy dw dh 2

renderCopy' (MkRenderer renderer) (MkTexture texture)
            Nothing Nothing
  = foreign FFI_C "GAME_drawTexture"
            (Ptr -> Ptr ->
             Int -> Int -> Int -> Int ->
             Int -> Int -> Int -> Int ->
             Int -> IO Int)
            renderer texture 0 0 0 0 0 0 0 0 0

-- renderCopy'' (MkRenderer renderer) (MkTexture texture)
--             (MkSDLRect sx sy sz sw) (MkSDLRect ds dt du dv) =
--   foreign FFI_C "GAME_drawTexture" (Ptr -> Ptr ->
--       Int -> Int -> Int -> Int -> Int -> Int -> Int -> Int -> IO Int)
--           renderer texture sx sy sz sw ds dt du dv

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
loadImage : (path : String) -> IO (Maybe Surface)
loadImage path = do
  surface <- foreign FFI_C "IMG_Load" (String -> IO Ptr) path
  if surface == null
    then pure Nothing
    else pure $ Just (MkSurface surface)

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

export
renderText : (renderer : Renderer) ->
             (text : String) ->
             (size : Int) ->
             (color : (Int, Int, Int, Int)) ->
             (rect : SDLRect) ->
             IO ()
renderText (MkRenderer ptr) text size (r, g, b, a) (MkSDLRect x y w h)
  = foreign FFI_C "renderText"
        (Ptr -> String -> Int -> Int -> Int -> Int -> Int ->
         Int -> Int -> Int -> Int -> IO ()) ptr text size r g b a x y w h

export
getTicks : IO Int
getTicks = foreign FFI_C "SDL_GetTicks" (IO Int)

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
           | Scroll Int Int

export
Show Event where
  show (KeyDown x) = "keydown"
  show (KeyUp x) = "keyup"
  show (MouseMotion x y z w) = "mousemotion " ++ show x ++ " " ++ show y
  show (MouseButtonDown x y z) = "mbd"
  show (MouseButtonUp x y z) = "mbu "
  show (Resize x y) = "resize " ++ show x ++ " " ++ show y
  show AppQuit = "AppQuit"
  show (Scroll x y) = "scroll " ++ show x ++ " " ++ show y

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
  (Scroll x y) == (Scroll x' y')
      = x == x' && y == y'
  _           == _           = False

export
pollEvent : IO (Maybe Event)
pollEvent = do
  vm <- getMyVM
  MkRaw e <- foreign FFI_C "pollEvent" (Ptr -> IO (Raw (Maybe Event))) vm
  pure e

reportEvent : Event -> IO ()
reportEvent (KeyDown x) = pure ()
reportEvent (KeyUp x) = pure ()
reportEvent (MouseMotion x y z w) = pure ()
reportEvent (MouseButtonDown x y z) = putStrLn $ "down: (" ++ show y ++ ", " ++ show z ++ ")"
reportEvent (MouseButtonUp x y z) = putStrLn $ "up: (" ++ show y ++ ", " ++ show z ++ ")"
reportEvent (Resize x y) = pure ()
reportEvent (Scroll x y) = pure ()
reportEvent AppQuit = pure ()

-- TODO move to C
export
pollEvents : IO (List Event)
pollEvents = pollEvents' [] where
  pollEvents' : List Event -> IO (List Event)
  pollEvents' acc = do
    vm <- getMyVM
    MkRaw e <- foreign FFI_C "pollEvent" (Ptr -> IO (Raw (Maybe Event))) vm
    -- putStrLn $ "event: " ++ show e
    case e of
         Nothing => pure acc -- weird hack, event has to be evaluated for scroll events to work
         Just event => pure (show event) >>= const (pollEvents' (event :: acc))


export
pollEventsForQuit : IO Bool
pollEventsForQuit = do
  quit <- foreign FFI_C "pollEventsForQuit" (IO Int)
  pure $ quit == 1

export
destroyTexture : Texture -> IO ()
destroyTexture (MkTexture ptr) = foreign FFI_C "destroyTexture" (Ptr -> IO ()) ptr
