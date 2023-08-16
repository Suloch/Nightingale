#pragma once

# define NE_SUCCESS 0
# define NE_FAILURE 1
#include <cstdlib>

namespace ne{

    typedef struct Application{

    };

    u_int32_t create_application(Application *app);
    u_int32_t run_application(Application app);

}

