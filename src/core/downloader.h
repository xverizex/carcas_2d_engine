#pragma once
#include "link.h"
#include "to_download.h"

Link *downloader_load (const enum TO_DOWNLOADS);
Link *downloader_text_get_link (uint8_t *data, int max_width, int max_height);
