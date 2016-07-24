module Main

import Graphics.SDL2 as SDL2
import System

fail : (msg: String) -> IO ()
fail msg = do
  err <- getError
  fPutStr stderr $ msg ++ " failed:" ++ err
  fflush stderr
  System.exit 1

main : IO ()
main = (do
  renderer <- SDL2.init 640 480
  loop renderer)
    where
      loop : SDL2.Renderer -> IO ()
      loop renderer = do
        -- False <- SDL2.pollEventsForQuit | pure () -- Hacky check for quit events.
        True <- SDL2.setRendererDrawColor renderer 0 0 111 255
          | fail "setRendererDrawColor"
        SDL2.renderClear renderer
        SDL2.filledRect renderer 100 100 50 50 255 0 0 128
        SDL2.renderPresent renderer
        event <- SDL2.pollEvent
        case event of
          Just (SDL2.AppQuit) => do
            putStrLn "AppQuit"
            pure ()
          Just (SDL2.KeyDown SDL2.KeyEsc) => do
            putStrLn ("KeyDown KeyEsc")
            pure ()
          _ => do loop renderer
