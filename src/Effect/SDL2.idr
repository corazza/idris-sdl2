module Effect.SDL2

import Effects
import public Graphics.SDL2

Rdr : Type
Rdr = Renderer

data Colour = MkCol Int Int Int Int

black : Colour
black = MkCol 0 0 0 255

white : Colour
white = MkCol 255 255 255 255

red : Colour
red = MkCol 255 0 0 255

green : Colour
green = MkCol 0 255 0 255

blue : Colour
blue = MkCol 0 0 255 255

yellow : Colour
yellow = MkCol 255 255 0 255

cyan : Colour
cyan = MkCol 0 255 255 255

magenta : Colour
magenta = MkCol 255 0 255 255

data Sdl : Effect where
     Initialise : Int -> Int -> Sdl () () (\v => Rdr)
     Quit : Sdl () Rdr (\v => ())
     Flip : Sdl () Rdr (\v => Rdr)
     Poll : Sdl (Maybe Event) a (\v => a)

     WithRenderer : (Rdr -> IO a) -> Sdl a Rdr (\v => Rdr)

Handler Sdl IO where
     handle () (Initialise x y) k = do srf <- SDL2.init x y; k () srf
     handle s Quit k = do SDL2.quit; k () ()

     handle r Flip k = do renderPresent r; k () r
     handle r Poll k = do x <- pollEvent; k x r
     handle r (WithRenderer f) k = do res <- f r; k res r

SDL : Type -> EFFECT
SDL res = MkEff res Sdl

SDL_ON : EFFECT
SDL_ON = SDL SDL2.Renderer

initialise : Int -> Int -> { [SDL ()] ==> [SDL_ON] } Eff () 
initialise x y = call $ Initialise x y

quit : { [SDL_ON] ==> [SDL ()] } Eff () 
quit = call Quit

flip : { [SDL_ON] } Eff ()
flip = call Flip

poll : { [SDL_ON] } Eff (Maybe Event) 
poll = call Poll

getRenderer : { [SDL_ON] } Eff SDL2.Renderer
getRenderer = call $ WithRenderer (\s => return s)

rectangle : Colour -> Int -> Int -> Int -> Int -> { [SDL_ON] } Eff () 
rectangle (MkCol r g b a) x y w h 
     = call $ WithRenderer (\s => filledRect s x y w h r g b a)

ellipse : Colour -> Int -> Int -> Int -> Int -> { [SDL_ON] } Eff () 
ellipse (MkCol r g b a) x y rx ry 
     = call $ WithRenderer (\s => filledEllipse s x y rx ry r g b a)

line : Colour -> Int -> Int -> Int -> Int -> { [SDL_ON] } Eff () 
line (MkCol r g b a) x y ex ey 
     = call $ WithRenderer (\s => drawLine s x y ex ey r g b a)
