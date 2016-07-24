module Main

import Graphics.SDL2 as SDL2
import System as System

abort : (msg: String) -> IO ()
abort msg = do
  err <- getError
  fPutStr stderr $ msg ++ " failed:" ++ err
  fflush stderr
  System.exit 1

main : IO ()
main = (do
  renderer <- SDL2.init 640 480
  bmp <- SDL2.loadBMP "saturn_1_640.bmp" -- XXX: handle errors
{-
  case bmp of
         Surface ptr => if ptr == null then abort "loadBMP" else pure ()
-}
  texture <- SDL2.createTextureFromSurface renderer bmp -- XXX: handle errors
  SDL2.freeSurface bmp
  loop renderer texture
  SDL2.destroyRenderer renderer
  SDL2.quit)
    where
      loop : Renderer -> (texture : Texture) -> IO ()
      loop renderer texture = do
        False <- SDL2.pollEventsForQuit | pure ()
        True <- SDL2.setRendererDrawColor renderer 0 0 0 0
          | abort "setRendererDrawColor"
        SDL2.renderClear renderer
        rc <- SDL2.renderCopy renderer texture -- draw texture
        when (rc /= 0) $ abort "renderCopy"
        SDL2.renderPresent renderer -- update screen
        loop renderer texture
