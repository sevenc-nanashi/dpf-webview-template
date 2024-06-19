<script setup lang="ts">
import { useParam1 } from "./bridge.ts";

const param1 = useParam1();
</script>

<template>
  <div>
    <h1>DPF Webview Template</h1>
    Here's the parameter:
    <input type="range" v-model="param1" step="0.1" />
    <label>{{ param1 }}</label>
  </div>
</template>

<style scoped lang="scss"></style>
