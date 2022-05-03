#pragma once

enum EVENT_TYPE {
	BUTTON_DOWN,
	BUTTON_UP
};

struct event {
	enum EVENT_TYPE type;
	int x;
	int y;
};
