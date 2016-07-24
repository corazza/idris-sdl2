module Main

import Graphics.SDL2 as SDL2
import System as System

fail : (msg: String) -> IO ()
fail msg = do
  err <- getError
  fPutStr stderr $ msg ++ " failed:" ++ err
  fflush stderr
  System.exit 1

width : Int
width = 640

height : Int
height = 480

squareSize : Int
squareSize = 50

main : IO ()
main = (do
  renderer <- SDL2.init width height
  loop renderer
  SDL2.destroyRenderer renderer
  quit)
    where
      loop : Renderer -> IO ()
      loop renderer = do
        False <- SDL2.pollEventsForQuit | pure ()
        True <- SDL2.setRendererDrawColor renderer 0 0 111 255
          | fail "setRendererDrawColor"
        SDL2.renderClear renderer
        SDL2.filledRect renderer  ((width `div` 2) - (squareSize `div` 2))
                                  ((height `div` 2) - (squareSize `div` 2))
                                  squareSize  squareSize  255  0  0  128
        SDL2.renderPresent renderer
        loop renderer
