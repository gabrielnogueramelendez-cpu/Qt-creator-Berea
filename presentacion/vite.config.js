import { defineConfig } from 'vite';
import { viteSingleFile } from 'vite-plugin-singlefile';

// Genera un único dist/index.html autocontenido (JS, CSS, fuentes e imágenes embebidas),
// para abrirlo con doble clic o proyectarlo sin conexión a internet.
export default defineConfig({
  base: './',
  plugins: [viteSingleFile()],
  build: { target: 'es2019', cssCodeSplit: false },
});
