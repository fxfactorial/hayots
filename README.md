hayots
------

native OCaml IDE on OS X.

Just type `make`, then try it with `./hayots.native`

# Build procedure

1. Need to `git clone https://github.com/mugginsoft/Fragaria`
2. `xcodebuild -project Fragaria.xcodeproj -alltargets -configuration Release`
3. Use framework in build
4. `cp -R MGSFragaria.framework ~/Library/Frameworks` (better with
   `install_name_tool`)
