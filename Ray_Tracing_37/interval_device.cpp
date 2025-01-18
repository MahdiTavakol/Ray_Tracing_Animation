#include "interval_device.h"

const interval_device interval_device::empty = interval_device(+infinity, -infinity);
const interval_device interval_device::universe = interval_device(-infinity, +infinity);

