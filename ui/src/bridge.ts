import { Plugin, inject, ref, computed, ComputedRef } from "vue";

declare function hostGetParameter(index: number): Promise<number>;
declare function hostSetParameter(index: number, value: number): Promise<void>;

export const getParam = (index: number) => {
  return hostGetParameter(index);
};

export const setParam = (index: number, value: number) => {
  hostSetParameter(index, value);
};

export const bridgePlugin: Plugin = {
  install(app) {
    const param1Value = ref(0);
    // @ts-expect-error It's required this function to expose globally
    window.parameterChanged = function (index: number, value: number) {
      if (index === 0) {
        param1Value.value = value;
      }
    };
    const param1 = computed({
      get: () => param1Value.value,
      set: (value) => {
        param1Value.value = value;
        setParam(0, value);
      },
    });

    getParam(0).then((value) => {
      param1Value.value = value;
    });
    app.provide("param1", param1);
  },
};

export const useParam1 = () => {
  return inject("param1") as ComputedRef<number>;
};
