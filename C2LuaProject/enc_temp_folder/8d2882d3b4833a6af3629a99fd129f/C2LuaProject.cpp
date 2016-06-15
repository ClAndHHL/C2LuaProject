// C2LuaProject.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <string.h>

static void stackDump(lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		const char* s;
		switch (t) {
		case LUA_TSTRING:  /* strings */
			s = lua_tostring(L, i);
			printf("%s", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:  /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:  /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		default:  /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf("  ");  /* put a separator */
	}
	printf("\n");     /* end the listing */
}

int main(void)
{
	char buff[256];
	int error;
	lua_State *L = lua_open();  /* opens Lua */
	luaopen_base(L);         /* opens the basic library */
	luaopen_table(L);        /* opens the table library */
	//luaopen_io(L);           /* opens the I/O library */
	luaopen_string(L);       /* opens the string lib. */
	luaopen_math(L);         /* opens the math lib. */

	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff),
			"line") || lua_pcall(L, 0, 1, 0);
		printf("call once\n");
		stackDump(L);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);/* pop error message from the stack */
		}
	}

	lua_close(L);
	return 0;
}



