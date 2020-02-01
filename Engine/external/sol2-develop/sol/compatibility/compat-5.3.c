#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include "compat-5.3.h"


#ifndef KEPLER_PROJECT_COMPAT53_C_
#define KEPLER_PROJECT_COMPAT53_C_




#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501

#ifndef COMPAT53_FOPEN_NO_LOCK
#  if defined(_MSC_VER)
#    define COMPAT53_FOPEN_NO_LOCK 1
#  else 
#    define COMPAT53_FOPEN_NO_LOCK 0
#  endif 
#endif 

#if defined(_MSC_VER) && COMPAT53_FOPEN_NO_LOCK
#  include <share.h>
#endif 

#ifndef COMPAT53_HAVE_STRERROR_R
#  if defined(__GLIBC__) || defined(_POSIX_VERSION) || defined(__APPLE__) || \
      (!defined (__MINGW32__) && defined(__GNUC__) && (__GNUC__ < 6))
#    define COMPAT53_HAVE_STRERROR_R 1
#  else 
#    define COMPAT53_HAVE_STRERROR_R 0
#  endif 
#endif 

#ifndef COMPAT53_HAVE_STRERROR_S
#  if defined(_MSC_VER) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
      (defined(__STDC_LIB_EXT1__) && __STDC_LIB_EXT1__)
#    define COMPAT53_HAVE_STRERROR_S 1
#  else 
#    define COMPAT53_HAVE_STRERROR_S 0
#  endif 
#endif 

#ifndef COMPAT53_LUA_FILE_BUFFER_SIZE
#  define COMPAT53_LUA_FILE_BUFFER_SIZE 4096
#endif 


static char* compat53_strerror(int en, char* buff, size_t sz) {
#if COMPAT53_HAVE_STRERROR_R
	
	if (sz > 0) {
		buff[0] = '\0';
		
		if (strerror_r(en, buff, sz)) {
			
		}
		if (buff[0] == '\0') {
			
			return strerror(en);
		}
	}
	return buff; 
#elif COMPAT53_HAVE_STRERROR_S
	
	strerror_s(buff, sz, en);
	return buff;
#else
	
	(void)buff;
	(void)sz;
	return strerror(en);
#endif
}


COMPAT53_API int lua_absindex(lua_State *L, int i) {
	if (i < 0 && i > LUA_REGISTRYINDEX)
		i += lua_gettop(L) + 1;
	return i;
}


static void compat53_call_lua(lua_State *L, char const code[], size_t len,
	int nargs, int nret) {
	lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)code);
	if (lua_type(L, -1) != LUA_TFUNCTION) {
		lua_pop(L, 1);
		if (luaL_loadbuffer(L, code, len, "=none"))
			lua_error(L);
		lua_pushvalue(L, -1);
		lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)code);
	}
	lua_insert(L, -nargs - 1);
	lua_call(L, nargs, nret);
}


static const char compat53_arith_code[] =
"local op,a,b=...\n"
"if op==0 then return a+b\n"
"elseif op==1 then return a-b\n"
"elseif op==2 then return a*b\n"
"elseif op==3 then return a/b\n"
"elseif op==4 then return a%b\n"
"elseif op==5 then return a^b\n"
"elseif op==6 then return -a\n"
"end\n";

COMPAT53_API void lua_arith(lua_State *L, int op) {
	if (op < LUA_OPADD || op > LUA_OPUNM)
		luaL_error(L, "invalid 'op' argument for lua_arith");
	luaL_checkstack(L, 5, "not enough stack slots");
	if (op == LUA_OPUNM)
		lua_pushvalue(L, -1);
	lua_pushnumber(L, op);
	lua_insert(L, -3);
	compat53_call_lua(L, compat53_arith_code,
		sizeof(compat53_arith_code) - 1, 3, 1);
}


static const char compat53_compare_code[] =
"local a,b=...\n"
"return a<=b\n";

COMPAT53_API int lua_compare(lua_State *L, int idx1, int idx2, int op) {
	int result = 0;
	switch (op) {
	case LUA_OPEQ:
		return lua_equal(L, idx1, idx2);
	case LUA_OPLT:
		return lua_lessthan(L, idx1, idx2);
	case LUA_OPLE:
		luaL_checkstack(L, 5, "not enough stack slots");
		idx1 = lua_absindex(L, idx1);
		idx2 = lua_absindex(L, idx2);
		lua_pushvalue(L, idx1);
		lua_pushvalue(L, idx2);
		compat53_call_lua(L, compat53_compare_code,
			sizeof(compat53_compare_code) - 1, 2, 1);
		result = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return result;
	default:
		luaL_error(L, "invalid 'op' argument for lua_compare");
	}
	return 0;
}


COMPAT53_API void lua_copy(lua_State *L, int from, int to) {
	int abs_to = lua_absindex(L, to);
	luaL_checkstack(L, 1, "not enough stack slots");
	lua_pushvalue(L, from);
	lua_replace(L, abs_to);
}


COMPAT53_API void lua_len(lua_State *L, int i) {
	switch (lua_type(L, i)) {
	case LUA_TSTRING:
		lua_pushnumber(L, (lua_Number)lua_objlen(L, i));
		break;
	case LUA_TTABLE:
		if (!luaL_callmeta(L, i, "__len"))
			lua_pushnumber(L, (lua_Number)lua_objlen(L, i));
		break;
	case LUA_TUSERDATA:
		if (luaL_callmeta(L, i, "__len"))
			break;
		
	default:
		luaL_error(L, "attempt to get length of a %s value",
			lua_typename(L, lua_type(L, i)));
	}
}


COMPAT53_API int lua_rawgetp(lua_State *L, int i, const void *p) {
	int abs_i = lua_absindex(L, i);
	lua_pushlightuserdata(L, (void*)p);
	lua_rawget(L, abs_i);
	return lua_type(L, -1);
}

COMPAT53_API void lua_rawsetp(lua_State *L, int i, const void *p) {
	int abs_i = lua_absindex(L, i);
	luaL_checkstack(L, 1, "not enough stack slots");
	lua_pushlightuserdata(L, (void*)p);
	lua_insert(L, -2);
	lua_rawset(L, abs_i);
}


COMPAT53_API lua_Number lua_tonumberx(lua_State *L, int i, int *isnum) {
	lua_Number n = lua_tonumber(L, i);
	if (isnum != NULL) {
		*isnum = (n != 0 || lua_isnumber(L, i));
	}
	return n;
}


COMPAT53_API void luaL_checkversion(lua_State *L) {
	(void)L;
}


COMPAT53_API void luaL_checkstack(lua_State *L, int sp, const char *msg) {
	if (!lua_checkstack(L, sp + LUA_MINSTACK)) {
		if (msg != NULL)
			luaL_error(L, "stack overflow (%s)", msg);
		else {
			lua_pushliteral(L, "stack overflow");
			lua_error(L);
		}
	}
}


COMPAT53_API int luaL_getsubtable(lua_State *L, int i, const char *name) {
	int abs_i = lua_absindex(L, i);
	luaL_checkstack(L, 3, "not enough stack slots");
	lua_pushstring(L, name);
	lua_gettable(L, abs_i);
	if (lua_istable(L, -1))
		return 1;
	lua_pop(L, 1);
	lua_newtable(L);
	lua_pushstring(L, name);
	lua_pushvalue(L, -2);
	lua_settable(L, abs_i);
	return 0;
}


COMPAT53_API lua_Integer luaL_len(lua_State *L, int i) {
	lua_Integer res = 0;
	int isnum = 0;
	luaL_checkstack(L, 1, "not enough stack slots");
	lua_len(L, i);
	res = lua_tointegerx(L, -1, &isnum);
	lua_pop(L, 1);
	if (!isnum)
		luaL_error(L, "object length is not an integer");
	return res;
}


COMPAT53_API void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
	luaL_checkstack(L, nup + 1, "too many upvalues");
	for (; l->name != NULL; l++) {  
		int i;
		lua_pushstring(L, l->name);
		for (i = 0; i < nup; i++)  
			lua_pushvalue(L, -(nup + 1));
		lua_pushcclosure(L, l->func, nup);  
		lua_settable(L, -(nup + 3)); 
	}
	lua_pop(L, nup);  
}


COMPAT53_API void luaL_setmetatable(lua_State *L, const char *tname) {
	luaL_checkstack(L, 1, "not enough stack slots");
	luaL_getmetatable(L, tname);
	lua_setmetatable(L, -2);
}


COMPAT53_API void *luaL_testudata(lua_State *L, int i, const char *tname) {
	void *p = lua_touserdata(L, i);
	luaL_checkstack(L, 2, "not enough stack slots");
	if (p == NULL || !lua_getmetatable(L, i))
		return NULL;
	else {
		int res = 0;
		luaL_getmetatable(L, tname);
		res = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!res)
			p = NULL;
	}
	return p;
}


static int compat53_countlevels(lua_State *L) {
	lua_Debug ar;
	int li = 1, le = 1;
	
	while (lua_getstack(L, le, &ar)) { li = le; le *= 2; }
	
	while (li < le) {
		int m = (li + le) / 2;
		if (lua_getstack(L, m, &ar)) li = m + 1;
		else le = m;
	}
	return le - 1;
}

static int compat53_findfield(lua_State *L, int objidx, int level) {
	if (level == 0 || !lua_istable(L, -1))
		return 0;  
	lua_pushnil(L);  
	while (lua_next(L, -2)) {  
		if (lua_type(L, -2) == LUA_TSTRING) {  
			if (lua_rawequal(L, objidx, -1)) {  
				lua_pop(L, 1);  
				return 1;
			}
			else if (compat53_findfield(L, objidx, level - 1)) {  
				lua_remove(L, -2);  
				lua_pushliteral(L, ".");
				lua_insert(L, -2);  
				lua_concat(L, 3);
				return 1;
			}
		}
		lua_pop(L, 1);  
	}
	return 0;  
}

static int compat53_pushglobalfuncname(lua_State *L, lua_Debug *ar) {
	int top = lua_gettop(L);
	lua_getinfo(L, "f", ar);  
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	if (compat53_findfield(L, top + 1, 2)) {
		lua_copy(L, -1, top + 1);  
		lua_pop(L, 2);  
		return 1;
	}
	else {
		lua_settop(L, top);  
		return 0;
	}
}

static void compat53_pushfuncname(lua_State *L, lua_Debug *ar) {
	if (*ar->namewhat != '\0')  
		lua_pushfstring(L, "function " LUA_QS, ar->name);
	else if (*ar->what == 'm')  
		lua_pushliteral(L, "main chunk");
	else if (*ar->what == 'C') {
		if (compat53_pushglobalfuncname(L, ar)) {
			lua_pushfstring(L, "function " LUA_QS, lua_tostring(L, -1));
			lua_remove(L, -2);  
		}
		else
			lua_pushliteral(L, "?");
	}
	else
		lua_pushfstring(L, "function <%s:%d>", ar->short_src, ar->linedefined);
}

#define COMPAT53_LEVELS1 12  
#define COMPAT53_LEVELS2 10  

COMPAT53_API void luaL_traceback(lua_State *L, lua_State *L1,
	const char *msg, int level) {
	lua_Debug ar;
	int top = lua_gettop(L);
	int numlevels = compat53_countlevels(L1);
	int mark = (numlevels > COMPAT53_LEVELS1 + COMPAT53_LEVELS2) ? COMPAT53_LEVELS1 : 0;
	if (msg) lua_pushfstring(L, "%s\n", msg);
	lua_pushliteral(L, "stack traceback:");
	while (lua_getstack(L1, level++, &ar)) {
		if (level == mark) {  
			lua_pushliteral(L, "\n\t...");  
			level = numlevels - COMPAT53_LEVELS2;  
		}
		else {
			lua_getinfo(L1, "Slnt", &ar);
			lua_pushfstring(L, "\n\t%s:", ar.short_src);
			if (ar.currentline > 0)
				lua_pushfstring(L, "%d:", ar.currentline);
			lua_pushliteral(L, " in ");
			compat53_pushfuncname(L, &ar);
			lua_concat(L, lua_gettop(L) - top);
		}
	}
	lua_concat(L, lua_gettop(L) - top);
}


COMPAT53_API int luaL_fileresult(lua_State *L, int stat, const char *fname) {
	const char *serr = NULL;
	int en = errno;  
	char buf[512] = { 0 };
	if (stat) {
		lua_pushboolean(L, 1);
		return 1;
	}
	else {
		lua_pushnil(L);
		serr = compat53_strerror(en, buf, sizeof(buf));
		if (fname)
			lua_pushfstring(L, "%s: %s", fname, serr);
		else
			lua_pushstring(L, serr);
		lua_pushnumber(L, (lua_Number)en);
		return 3;
	}
}


static int compat53_checkmode(lua_State *L, const char *mode, const char *modename, int err) {
	if (mode && strchr(mode, modename[0]) == NULL) {
		lua_pushfstring(L, "attempt to load a %s chunk (mode is '%s')", modename, mode);
		return err;
	}
	return LUA_OK;
}


typedef struct {
	lua_Reader reader;
	void *ud;
	int has_peeked_data;
	const char *peeked_data;
	size_t peeked_data_size;
} compat53_reader_data;


static const char *compat53_reader(lua_State *L, void *ud, size_t *size) {
	compat53_reader_data *data = (compat53_reader_data *)ud;
	if (data->has_peeked_data) {
		data->has_peeked_data = 0;
		*size = data->peeked_data_size;
		return data->peeked_data;
	}
	else
		return data->reader(L, data->ud, size);
}


COMPAT53_API int lua_load(lua_State *L, lua_Reader reader, void *data, const char *source, const char *mode) {
	int status = LUA_OK;
	compat53_reader_data compat53_data = { reader, data, 1, 0, 0 };
	compat53_data.peeked_data = reader(L, data, &(compat53_data.peeked_data_size));
	if (compat53_data.peeked_data && compat53_data.peeked_data_size &&
		compat53_data.peeked_data[0] == LUA_SIGNATURE[0]) 
		status = compat53_checkmode(L, mode, "binary", LUA_ERRSYNTAX);
	else
		status = compat53_checkmode(L, mode, "text", LUA_ERRSYNTAX);
	if (status != LUA_OK)
		return status;
	
#undef lua_load
	return lua_load(L, compat53_reader, &compat53_data, source);
#define lua_load COMPAT53_CONCAT(COMPAT53_PREFIX, _load_53)
}


typedef struct {
	int n;  
	FILE *f;  
	char buff[COMPAT53_LUA_FILE_BUFFER_SIZE];  
} compat53_LoadF;


static const char *compat53_getF(lua_State *L, void *ud, size_t *size) {
	compat53_LoadF *lf = (compat53_LoadF *)ud;
	(void)L;  
	if (lf->n > 0) {  
		*size = lf->n;  
		lf->n = 0;  
	}
	else {  
		   
		if (feof(lf->f)) return NULL;
		*size = fread(lf->buff, 1, sizeof(lf->buff), lf->f);  
	}
	return lf->buff;
}


static int compat53_errfile(lua_State *L, const char *what, int fnameindex) {
	char buf[512] = { 0 };
	const char *serr = compat53_strerror(errno, buf, sizeof(buf));
	const char *filename = lua_tostring(L, fnameindex) + 1;
	lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
	lua_remove(L, fnameindex);
	return LUA_ERRFILE;
}


static int compat53_skipBOM(compat53_LoadF *lf) {
	const char *p = "\xEF\xBB\xBF";  
	int c;
	lf->n = 0;
	do {
		c = getc(lf->f);
		if (c == EOF || c != *(const unsigned char *)p++) return c;
		lf->buff[lf->n++] = (char)c;  
	} while (*p != '\0');
	lf->n = 0;  
	return getc(lf->f);  
}



static int compat53_skipcomment(compat53_LoadF *lf, int *cp) {
	int c = *cp = compat53_skipBOM(lf);
	if (c == '#') {  
		do {  
			c = getc(lf->f);
		} while (c != EOF && c != '\n');
		*cp = getc(lf->f);  
		return 1;  
	}
	else return 0;  
}


COMPAT53_API int luaL_loadfilex(lua_State *L, const char *filename, const char *mode) {
	compat53_LoadF lf;
	int status, readstatus;
	int c;
	int fnameindex = lua_gettop(L) + 1;  
	if (filename == NULL) {
		lua_pushliteral(L, "=stdin");
		lf.f = stdin;
	}
	else {
		lua_pushfstring(L, "@%s", filename);
#if defined(_MSC_VER)
		

		
#if COMPAT53_FOPEN_NO_LOCK
		lf.f = _fsopen(filename, "r", _SH_DENYNO); 
		if (lf.f == NULL)
			return compat53_errfile(L, "open", fnameindex);
#else 
		if (fopen_s(&lf.f, filename, "r") != 0)
			return compat53_errfile(L, "open", fnameindex);
#endif 
#else
		lf.f = fopen(filename, "r"); 
		if (lf.f == NULL) return compat53_errfile(L, "open", fnameindex);
#endif
	}
	if (compat53_skipcomment(&lf, &c))  
		lf.buff[lf.n++] = '\n';  
	if (c == LUA_SIGNATURE[0] && filename) {  
#if defined(_MSC_VER)
		if (freopen_s(&lf.f, filename, "rb", lf.f) != 0)
			return compat53_errfile(L, "reopen", fnameindex);
#else
		lf.f = freopen(filename, "rb", lf.f);  
		if (lf.f == NULL) return compat53_errfile(L, "reopen", fnameindex);
#endif
		compat53_skipcomment(&lf, &c);  
	}
	if (c != EOF)
		lf.buff[lf.n++] = (char)c;  
	status = lua_load(L, &compat53_getF, &lf, lua_tostring(L, -1), mode);
	readstatus = ferror(lf.f);
	if (filename) fclose(lf.f);  
	if (readstatus) {
		lua_settop(L, fnameindex);  
		return compat53_errfile(L, "read", fnameindex);
	}
	lua_remove(L, fnameindex);
	return status;
}


COMPAT53_API int luaL_loadbufferx(lua_State *L, const char *buff, size_t sz, const char *name, const char *mode) {
	int status = LUA_OK;
	if (sz > 0 && buff[0] == LUA_SIGNATURE[0]) {
		status = compat53_checkmode(L, mode, "binary", LUA_ERRSYNTAX);
	}
	else {
		status = compat53_checkmode(L, mode, "text", LUA_ERRSYNTAX);
	}
	if (status != LUA_OK)
		return status;
	return luaL_loadbuffer(L, buff, sz, name);
}


#if !defined(l_inspectstat) && \
    (defined(unix) || defined(__unix) || defined(__unix__) || \
     defined(__TOS_AIX__) || defined(_SYSTYPE_BSD) || \
     (defined(__APPLE__) && defined(__MACH__)))

#  include <unistd.h>

#  if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
#    include <sys/wait.h>
#    define l_inspectstat(stat,what) \
  if (WIFEXITED(stat)) { stat = WEXITSTATUS(stat); } \
  else if (WIFSIGNALED(stat)) { stat = WTERMSIG(stat); what = "signal"; }
#  endif
#endif


#if !defined(l_inspectstat)
#  define l_inspectstat(stat,what) ((void)0)
#endif


COMPAT53_API int luaL_execresult(lua_State *L, int stat) {
	const char *what = "exit";
	if (stat == -1)
		return luaL_fileresult(L, 0, NULL);
	else {
		l_inspectstat(stat, what);
		if (*what == 'e' && stat == 0)
			lua_pushboolean(L, 1);
		else
			lua_pushnil(L);
		lua_pushstring(L, what);
		lua_pushinteger(L, stat);
		return 3;
	}
}


COMPAT53_API void luaL_buffinit(lua_State *L, luaL_Buffer_53 *B) {
	
	B->b.p = NULL;
	B->b.L = NULL;
	B->b.lvl = 0;
	
	B->ptr = B->b.buffer;
	B->capacity = LUAL_BUFFERSIZE;
	B->nelems = 0;
	B->L2 = L;
}


COMPAT53_API char *luaL_prepbuffsize(luaL_Buffer_53 *B, size_t s) {
	if (B->capacity - B->nelems < s) { 
		char* newptr = NULL;
		size_t newcap = B->capacity * 2;
		if (newcap - B->nelems < s)
			newcap = B->nelems + s;
		if (newcap < B->capacity) 
			luaL_error(B->L2, "buffer too large");
		newptr = (char*)lua_newuserdata(B->L2, newcap);
		memcpy(newptr, B->ptr, B->nelems);
		if (B->ptr != B->b.buffer)
			lua_replace(B->L2, -2); 
		B->ptr = newptr;
		B->capacity = newcap;
	}
	return B->ptr + B->nelems;
}


COMPAT53_API void luaL_addlstring(luaL_Buffer_53 *B, const char *s, size_t l) {
	memcpy(luaL_prepbuffsize(B, l), s, l);
	luaL_addsize(B, l);
}


COMPAT53_API void luaL_addvalue(luaL_Buffer_53 *B) {
	size_t len = 0;
	const char *s = lua_tolstring(B->L2, -1, &len);
	if (!s)
		luaL_error(B->L2, "cannot convert value to string");
	if (B->ptr != B->b.buffer)
		lua_insert(B->L2, -2); 
	luaL_addlstring(B, s, len);
	lua_remove(B->L2, B->ptr != B->b.buffer ? -2 : -1);
}


void luaL_pushresult(luaL_Buffer_53 *B) {
	lua_pushlstring(B->L2, B->ptr, B->nelems);
	if (B->ptr != B->b.buffer)
		lua_replace(B->L2, -2); 
}


#endif 




#if defined( LUA_VERSION_NUM ) && LUA_VERSION_NUM <= 502


COMPAT53_API int lua_geti(lua_State *L, int index, lua_Integer i) {
	index = lua_absindex(L, index);
	lua_pushinteger(L, i);
	lua_gettable(L, index);
	return lua_type(L, -1);
}


COMPAT53_API int lua_isinteger(lua_State *L, int index) {
	if (lua_type(L, index) == LUA_TNUMBER) {
		lua_Number n = lua_tonumber(L, index);
		lua_Integer i = lua_tointeger(L, index);
		if (i == n)
			return 1;
	}
	return 0;
}


COMPAT53_API lua_Integer lua_tointegerx(lua_State *L, int i, int *isnum) {
	int ok = 0;
	lua_Number n = lua_tonumberx(L, i, &ok);
	if (ok) {
		if (n == (lua_Integer)n) {
			if (isnum)
				*isnum = 1;
			return (lua_Integer)n;
		}
	}
	if (isnum)
		*isnum = 0;
	return 0;
}


static void compat53_reverse(lua_State *L, int a, int b) {
	for (; a < b; ++a, --b) {
		lua_pushvalue(L, a);
		lua_pushvalue(L, b);
		lua_replace(L, a);
		lua_replace(L, b);
	}
}


COMPAT53_API void lua_rotate(lua_State *L, int idx, int n) {
	int n_elems = 0;
	idx = lua_absindex(L, idx);
	n_elems = lua_gettop(L) - idx + 1;
	if (n < 0)
		n += n_elems;
	if (n > 0 && n < n_elems) {
		luaL_checkstack(L, 2, "not enough stack slots available");
		n = n_elems - n;
		compat53_reverse(L, idx, idx + n - 1);
		compat53_reverse(L, idx + n, idx + n_elems - 1);
		compat53_reverse(L, idx, idx + n_elems - 1);
	}
}


COMPAT53_API void lua_seti(lua_State *L, int index, lua_Integer i) {
	luaL_checkstack(L, 1, "not enough stack slots available");
	index = lua_absindex(L, index);
	lua_pushinteger(L, i);
	lua_insert(L, -2);
	lua_settable(L, index);
}


#if !defined(lua_str2number)
#  define lua_str2number(s, p)  strtod((s), (p))
#endif

COMPAT53_API size_t lua_stringtonumber(lua_State *L, const char *s) {
	char* endptr;
	lua_Number n = lua_str2number(s, &endptr);
	if (endptr != s) {
		while (*endptr != '\0' && isspace((unsigned char)*endptr))
			++endptr;
		if (*endptr == '\0') {
			lua_pushnumber(L, n);
			return endptr - s + 1;
		}
	}
	return 0;
}


COMPAT53_API const char *luaL_tolstring(lua_State *L, int idx, size_t *len) {
	if (!luaL_callmeta(L, idx, "__tostring")) {
		int t = lua_type(L, idx), tt = 0;
		char const* name = NULL;
		switch (t) {
		case LUA_TNIL:
			lua_pushliteral(L, "nil");
			break;
		case LUA_TSTRING:
		case LUA_TNUMBER:
			lua_pushvalue(L, idx);
			break;
		case LUA_TBOOLEAN:
			if (lua_toboolean(L, idx))
				lua_pushliteral(L, "true");
			else
				lua_pushliteral(L, "false");
			break;
		default:
			tt = luaL_getmetafield(L, idx, "__name");
			name = (tt == LUA_TSTRING) ? lua_tostring(L, -1) : lua_typename(L, t);
			lua_pushfstring(L, "%s: %p", name, lua_topointer(L, idx));
			if (tt != LUA_TNIL)
				lua_replace(L, -2);
			break;
		}
	}
	else {
		if (!lua_isstring(L, -1))
			luaL_error(L, "'__tostring' must return a string");
	}
	return lua_tolstring(L, -1, len);
}


COMPAT53_API void luaL_requiref(lua_State *L, const char *modname,
	lua_CFunction openf, int glb) {
	luaL_checkstack(L, 3, "not enough stack slots available");
	luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
	if (lua_getfield(L, -1, modname) == LUA_TNIL) {
		lua_pop(L, 1);
		lua_pushcfunction(L, openf);
		lua_pushstring(L, modname);
		lua_call(L, 1, 1);
		lua_pushvalue(L, -1);
		lua_setfield(L, -3, modname);
	}
	if (glb) {
		lua_pushvalue(L, -1);
		lua_setglobal(L, modname);
	}
	lua_replace(L, -2);
}


#endif 


#endif 




