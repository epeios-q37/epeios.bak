# *ZNDq* TODO

- Fix the *SegFault* which occurs when the component's dynamic library is unloaded by the `dlbrry::rDynamicLibrary` destructor (there is currently a workaround to avoid these).
- The *ZNDq*'s locale and configuration files have currently to be in the directory from where the program is launched. If *ZNDq* could fetch its location, this files could be placed on this location.
- Implement the equivalent of `die(...)` to be called from extension (`sclznd::Die(...)`).
