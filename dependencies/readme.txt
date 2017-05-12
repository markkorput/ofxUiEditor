\author Mark van de Korput

You can completely ignore the submodules in this folder; you don't even need to check them out
(so you don't need to run git submodule init and git submodule update, etc.)

The only reason they are added is to specify exactly which version (commit) of the addons we are using in this project,
so if there is any trouble installing these dependencies you could try to checkout exactly the version submodules referenced by the submodules.

During development you could link these submodules to the folders in your OF addons folder. See these instructions;
http://stackoverflow.com/questions/27379818/git-possible-to-use-same-submodule-working-copy-by-multiple-projects
