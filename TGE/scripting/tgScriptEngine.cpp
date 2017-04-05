#include "tgScriptEngine.h"

#include "../math/tgMath.h"
#include "../math/tgQuaternion.h"

// Helpers for overloaded functions
#define FUN_GET(type, fun, ret) static_cast<ret(type::*)()const>(&type::fun)
#define FUN_SET(type, fun, ret) static_cast<ret&(type::*)()>(&type::fun)

#define FUN1(type, fun, ret, ...) static_cast<ret(type::*)(__VA_ARGS__)const>(&type::fun)
#define FUN2(type, fun, ret, ...) static_cast<ret(type::*)(__VA_ARGS__)>(&type::fun)
 
#define FUNS(fun, ret, ...) static_cast<ret(*)(__VA_ARGS__)>(&fun)

tgScriptEngine::tgScriptEngine() {
	////////// Register math types
	L.new_usertype<tgVector2>(
		"Vector2",

		// CTor
		sol::constructors<tgVector2(), tgVector2(float), tgVector2(float, float)>(),

		// Properties
		"x", sol::property(
				FUN_GET(tgVector2, x, float),
				FUN_SET(tgVector2, x, float)
			),
		"y", sol::property(
				FUN_GET(tgVector2, y, float),
				FUN_SET(tgVector2, y, float)
			),
		"xx", sol::readonly_property(&tgVector2::xx),
		"yy", sol::readonly_property(&tgVector2::yy),
		"st", sol::readonly_property(&tgVector2::st),
		"length", sol::readonly_property(&tgVector2::length),
		"lengthSqr", sol::readonly_property(&tgVector2::lengthSquared)
		,

		// Functions
		"normalized", &tgVector2::normalized,
		"dot", &tgVector2::dot,
		"reflect", &tgVector2::reflect,
		"clamp", &tgVector2::clamp
		,

		// Operators
		"__len", &tgVector2::length,
		"__add", sol::overload(
			FUN1(tgVector2, operator+, tgVector2, tgVector2 const&),
			FUN1(tgVector2, operator+, tgVector2, float)
		),
		"__sub", sol::overload(
			FUN1(tgVector2, operator-, tgVector2, tgVector2 const&),
			FUN1(tgVector2, operator-, tgVector2, float)
		),
		"__mul", sol::overload(
			FUN1(tgVector2, operator*, tgVector2, tgVector2 const&),
			FUN1(tgVector2, operator*, tgVector2, float)
		),
		"__div", sol::overload(
			FUN1(tgVector2, operator/, tgVector2, tgVector2 const&),
			FUN1(tgVector2, operator/, tgVector2, float)
		)
	);

	L.new_usertype<tgVector3>(
		"Vector3",

		// CTor
		sol::constructors<tgVector3(), tgVector3(float), tgVector3(float, float, float), tgVector3(tgVector2 const&, float)>(),

		// Properties
		"x", sol::property(
			FUN_GET(tgVector3, x, float),
			FUN_SET(tgVector3, x, float)
			),
		"y", sol::property(
			FUN_GET(tgVector3, y, float),
			FUN_SET(tgVector3, y, float)
			),
		"z", sol::property(
			FUN_GET(tgVector3, z, float),
			FUN_SET(tgVector3, z, float)
			),
		"xx", sol::readonly_property(&tgVector3::xx),
		"yy", sol::readonly_property(&tgVector3::yy),
		"st", sol::readonly_property(&tgVector3::st),
		"str", sol::readonly_property(&tgVector3::str),
		"length", sol::readonly_property(&tgVector3::length),
		"lengthSqr", sol::readonly_property(&tgVector3::lengthSquared),

		// Functions
		"normalized", &tgVector3::normalized,
		"dot", &tgVector3::dot,
		"reflect", &tgVector3::reflect,
		"cross", &tgVector3::cross,
		"clamp", &tgVector3::clamp,

		// Operators
		"__len", &tgVector3::length,
		"__add", sol::overload(
			FUN1(tgVector3, operator+, tgVector3, tgVector3 const&),
			FUN1(tgVector3, operator+, tgVector3, float)
		),
		"__sub", sol::overload(
			FUN1(tgVector3, operator-, tgVector3, tgVector3 const&),
			FUN1(tgVector3, operator-, tgVector3, float)
		),
		"__mul", sol::overload(
			FUN1(tgVector3, operator*, tgVector3, tgVector3 const&),
			FUN1(tgVector3, operator*, tgVector3, float)
		),
		"__div", sol::overload(
			FUN1(tgVector3, operator/, tgVector3, tgVector3 const&),
			FUN1(tgVector3, operator/, tgVector3, float)
		)
	);

	L.new_usertype<tgVector4>(
		"Vector4",

		// CTor
		sol::constructors<tgVector4(), tgVector4(float), tgVector4(float, float, float, float), tgVector4(tgVector3 const&, float)>(),

		// Properties
		"x", sol::property(
			FUN_GET(tgVector4, x, float),
			FUN_SET(tgVector4, x, float)
			),
		"y", sol::property(
			FUN_GET(tgVector4, y, float),
			FUN_SET(tgVector4, y, float)
			),
		"z", sol::property(
			FUN_GET(tgVector4, z, float),
			FUN_SET(tgVector4, z, float)
			),
		"w", sol::property(
			FUN_GET(tgVector4, w, float),
			FUN_SET(tgVector4, w, float)
			),
		"xx", sol::readonly_property(&tgVector4::xx),
		"yy", sol::readonly_property(&tgVector4::yy),
		"st", sol::readonly_property(&tgVector4::st),
		"str", sol::readonly_property(&tgVector4::str),
		"xyww", sol::readonly_property(&tgVector4::xyww),
		"length", sol::readonly_property(&tgVector4::length),
		"lengthSqr", sol::readonly_property(&tgVector4::lengthSquared),

		// Functions
		"normalized", &tgVector4::normalized,
		"dot", &tgVector4::dot,
		"reflect", &tgVector4::reflect,
		"clamp", &tgVector4::clamp,

		// Operators
		"__len", &tgVector4::length,
		"__add", sol::overload(
			FUN1(tgVector4, operator+, tgVector4, tgVector4 const&),
			FUN1(tgVector4, operator+, tgVector4, float)
		),
		"__sub", sol::overload(
			FUN1(tgVector4, operator-, tgVector4, tgVector4 const&),
			FUN1(tgVector4, operator-, tgVector4, float)
		),
		"__mul", sol::overload(
			FUN1(tgVector4, operator*, tgVector4, tgVector4 const&),
			FUN1(tgVector4, operator*, tgVector4, float)
		),
		"__div", sol::overload(
			FUN1(tgVector4, operator/, tgVector4, tgVector4 const&),
			FUN1(tgVector4, operator/, tgVector4, float)
		)
	);

	L.new_usertype<tgMatrix4>(
		"Matrix4",

		// Ctor
		sol::constructors<tgMatrix4()>(),

		// Functions
		"determinant", &tgMatrix4::determinant,
		"inverted", &tgMatrix4::inverted,
		"transposed", &tgMatrix4::transposed,
		"get", &tgMatrix4::get,
		"set", &tgMatrix4::set,

		// Operators
		"__mul", sol::overload(
			FUN2(tgMatrix4, operator*, tgMatrix4, tgMatrix4 const&),
			FUN2(tgMatrix4, operator*, tgVector4, tgVector4 const&),
			FUN2(tgMatrix4, operator*, tgVector3, tgVector3 const&),
			FUN2(tgMatrix4, operator*, tgMatrix4, float)
		),

		// Statics
		"identity", &tgMatrix4::identity,
		"translation", &tgMatrix4::translation,
		"rotation", sol::overload(
			FUNS(tgMatrix4::rotation, tgMatrix4, float, tgVector3 const&),
			FUNS(tgMatrix4::rotation, tgMatrix4, tgVector3 const&, tgVector3 const&, tgVector3 const&)
		),
		"scaling", &tgMatrix4::scaling,
		"perspective", &tgMatrix4::perspective,
		"ortho", &tgMatrix4::ortho,
		"lookAt", &tgMatrix4::lookAt
	);

	L.new_usertype<tgQuaternion>(
		"Quaternion",

		// CTor
		sol::constructors<
			tgQuaternion(),
			tgQuaternion(tgVector3 const&, float),
			tgQuaternion(float, float, float, float),
			tgQuaternion(tgQuaternion const&)
		>(),

		// Properties
		"x", sol::property(
			FUN_GET(tgQuaternion, x, float),
			FUN_SET(tgQuaternion, x, float)
			),
		"y", sol::property(
			FUN_GET(tgQuaternion, y, float),
			FUN_SET(tgQuaternion, y, float)
			),
		"z", sol::property(
			FUN_GET(tgQuaternion, z, float),
			FUN_SET(tgQuaternion, z, float)
			),
		"w", sol::property(
			FUN_GET(tgQuaternion, w, float),
			FUN_SET(tgQuaternion, w, float)
			),
		"length", sol::readonly_property(&tgQuaternion::length),

		// Functions
		"normalized", &tgQuaternion::normalized,
		"dot", &tgQuaternion::dot,
		"rotateVector3", &tgQuaternion::rotateVector3,
		"toMatrix", &tgQuaternion::toMatrix,
		"toEuler", &tgQuaternion::toEuler,

		// Operators
		"__len", &tgQuaternion::length,
		"__add", &tgQuaternion::operator+,
		"__sub", &tgQuaternion::operator-,
		"__mul", sol::overload(
			FUN1(tgQuaternion, operator*, tgQuaternion, float),
			FUN1(tgQuaternion, operator*, tgQuaternion, tgQuaternion const&),
			FUN1(tgQuaternion, operator*, tgQuaternion, tgVector3 const&)
		)
	);

	////////// Register core types
	
}

tgScriptEngine::~tgScriptEngine() {
}
