#pragma once
#define panic(message) { return 1; }
#define error(expression, message) { if(expression) panic(message); }
