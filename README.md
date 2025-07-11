# NFS The Run Modloader
Yeah that's it! A mod loader for NFS The Run.

> [!IMPORTANT]
> This is a WIP (Work In Progress) project. That means it doesn't work yet. I'm (or maybe soon we're) working hard
> to make this thing work. If you know something about reverse engineering and you want to help, well, you're very welcome my friend.

## Development frozen
For now development of this project is frozen. I need to gather some internal energy and finish other ones I'm working on.

## How does it work
Basically it imitates the dxgi.dll library loaded by the game when it's starting. It exports a dummy CreateDXGIFactory2 function
that injects loader code and then calls the actual function.

## Installation guide (initial one)
1. Navigate to the game's directory
2. Copy the mod file (dxgi.dll) you've got and replace the original one in the game's directory
3. Start the game

## TODO/Roadmap
- (When loader is compiled as debug target) it's worth to enable NFS The Run Debug Mode that is confirmed to exist by [research](https://github.com/mRally2/The-Run-Tools-Research.git) conducted by @_mRally2
- There's a lot to do right now in terms of reverse engineering. For now the game has to be reverse engineered to find structures that are responsible
for events to respond to them.
- Create some sort of mod framework after majority of useful functions game/engine are found and hooked up
- After all of that I've been considering to add a built-in mod that fixes that terrible particle bugs and muffled sound when the game is running without framerate limit
- Patch built-in patch for *"Running on VM"* error on newer builds of Windows 11

## Development information that might be relevant
- I'm working on Win11 24H4 ([tiny11](https://github.com/ntdevlabs/tiny11builder))
- Currently working with 1.1.0.0 version of game's executable so if I find something, it's going to be for this version.

## My current research
My findings are located in [research/research.md](./research/research.md) file

> [!NOTE]
> I'm not a reverse engineering specialist. I'm more like a programmer with an idea to create something that could
> save my first NFS I have ever played. So I'm learning and developing in my free time.
