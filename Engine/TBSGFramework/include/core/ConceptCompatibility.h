#pragma once

#if __cplusplus >= 202000L

#define CONCEPT concept

#define CONCEPT_ARG(concept_name) concept_name

#define REQUIRES(...) [](__VA_ARGS__)
#define CONCEPT_FUNCTION(return_type, expression) { expression } -> return_type

#else

#define CONCEPT constexpr bool

#define CONCEPT_TYPE(concept_name) typename 

#define REQUIRES(...) [](__VA_ARGS__)

#define CONCEPT_FUNCTION(return_type, expression) if(!std::is_same_v<return_type, decltype(expression)>) return false

#endif
