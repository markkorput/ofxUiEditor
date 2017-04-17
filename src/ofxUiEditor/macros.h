#pragma once

#define OFX_UI_EDITOR_INSTANTIATOR(x) ([](){ return make_shared<x>(); })
