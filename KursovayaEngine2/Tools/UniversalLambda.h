#pragma once

//slower than std::function but more efficient on memory
template<typename ReturnType, typename...InputTypes>
class UniversalLambda {

	struct TunnelToFunctionHolder {
		virtual ReturnType operator()(InputTypes...params) = 0;
		virtual ~TunnelToFunctionHolder() = default;

	};
	template<typename LambdaType>
	struct FunctionHolder : TunnelToFunctionHolder {
		LambdaType Func;
		FunctionHolder(LambdaType&& func) :Func(std::move(func)) {

		}
		virtual ~FunctionHolder() = default;
		virtual ReturnType operator()(InputTypes...params) {
			return Func(params...);
		}
	};

	TunnelToFunctionHolder* TunnelPtr;
public:
	template<typename funcT>
	SimpleLambda(funcT&& func) {
		TunnelPtr = new FunctionHolder<funcT>(std::move(func));
	}
	~SimpleLambda() {
		delete TunnelPtr;
	}
	ReturnType operator()(InputTypes...params) { return (*TunnelPtr)(params...); }
};