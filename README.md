# SafeCall

Safely calls an Android native function, guaranteed function crashes at most once.

## Usages

### Dependency

Add the following code block to the project's `build.gradle` or `settings.gradle` (gradle7.0+).

```groovy
repositories {
    ...
    maven { url 'https://jitpack.io' }
}
```

Add the following code block to the module's `build.gradle`.

```groovy
android {
    ...
    buildFeatures {
        prefab true
    }
}

dependencies {
    implementation 'com.github.luoyesiqiu:SafeCall:1.0.0'
}
```

Add following code block to `CMakeLists.txt`.

```cmake
find_package(safecall REQUIRED CONFIG)
target_link_libraries(myapp safecall::safecall)
```

Includes this header file in your project.

```c
#include "safe_call.h"
```

### Call a has a return value function.

```c
void problem_call() {
    //This function may throw some crashes.
    ...
}

SAFE_CALL_VOID(problem_call);
```

### Call a has no return value function.

```c
int problem_call(int arg) {
    //This function may throw some crashes.
    ...
    return 0;
}
int ret = 0;
SAFE_CALL(problem_call,ret,123);
```

