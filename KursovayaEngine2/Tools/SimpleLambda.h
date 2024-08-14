#pragma once
template<typename FuncTyp>
struct SimpleLambda {
	FuncTyp Func;
	SimpleLambda(const FuncTyp&& func):Func(std::move(func)) {
		
	}
};