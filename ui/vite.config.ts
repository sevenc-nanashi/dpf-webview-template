import { Plugin, defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import archiver from "archiver";
import { createWriteStream } from "fs";
import { pipeline } from "stream/promises";

// https://vitejs.dev/config/
export default defineConfig(() => {
  return {
    plugins: [vue(), zipArchivePlugin],
  };
});

const zipArchivePlugin: Plugin = {
  name: "zip-archive",
  apply: "build",
  enforce: "post",
  async generateBundle(_options, bundle) {
    const archive = archiver("zip", { zlib: { level: 9 } });
    const output = createWriteStream(`${import.meta.dirname}/../src/ui.zip`);

    for (const file in bundle) {
      const bundleFile = bundle[file];
      const content =
        bundleFile.type === "asset" ? bundleFile.source : bundleFile.code;
      archive.append(Buffer.from(content), { name: file });
    }

    const pipelinePromise = pipeline(archive, output);

    archive.finalize();

    await pipelinePromise;
  },
};
