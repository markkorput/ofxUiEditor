#pragma once

#define OFX_UI_INSTANTIATOR(x) ([](){ return make_shared<x>(); })
