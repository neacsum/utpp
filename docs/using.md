# Using UTPP with real world projects #
Following is how I have been using UTTP in my own projects. It may not be the
best solution but it has worked for me.

## Assumptions ##
+ All the devlopment is in a folder `\development`
+ The project that needs to use UTPP is called COOL_PROJ and its path is
`\development\cool_proj`
+ Include files for COOL_PROJ are under `\development\cool_proj\include`
+ Library files used by COOL_PROJ are under `\development\cool_proj\lib`
+ UTPP repo has been downloaded in folder `\development\utpp`

## Steps ##

1. Create a symlink named `utpp` in `\development\cool_proj\include` pointing to
`\development\utpp\include\utpp`:
```
  mklink /d \development\cool_proj\include\utpp \development\utpp\include\utpp
```

2. Create a symlink named `lib` <b>in `\development\utpp`</b> pointing to `\development\cool_proj\lib`
```
  mklink /d \development\utpp\lib \development\cool_proj\lib
```
  Note that the two symlinks are pointing in opposite directions: the first one
  goes from cool_proj to utpp while the second one goes from utpp to cool_proj

3. Build the UTPP project. This should produce utpp.lib file in `\development\utpp\lib`
folder. Because of the second symlink, uttp.lib will show up in `\development\cool_proj\lib`.

4. In your test files place an include directive for UTPP stuff:
```
  #include <utpp/utpp.h>
```
  Because of the first symlink, `\development\cool_proj\include` contains a folder
  named `utpp` and the compiler can find the include file.

5. Build your test program. `utpp.h` file contains a #pragma directive that specifies
the `utpp.lib` and the linker will be able locate `utpp.lib` file in `\development\cool_proj\lib`

## Different Configurations ##
UTPP libarary can be built in debug or release configurations for 32 or 64 bits.
To accommodate that, the output library is produced in `lib\win32\debug` or `lib\x64\debug`
or `lib\win32\release` or `lib\x64\release`. If you follow the same convention for
COOL_PROJ everything should work automatically.

## FAQ ##
1. Does this work with git?

  Yes it does. Git tries not to touch folders and files that
  are not under source control. The way the symlinks are setup should take care of that.

2. What if I have more than one project that uses UTPP? How can I setup my symlinks?

  I find it easiest to have a single include folder with subfolders for each project
  (something like \development\include\cool_proj, \develompent\include\even_cooler, etc.)
  and **one common** lib folder. Follow the steps above and all should work smoothly.
