# smartledz-protocol

Smart LEDZ protocol v1 codec and command library extracted from `esphome-smart-ledz`.

## Public Headers

- `smartledz_protocol/v1/commands.h`
- `smartledz_protocol/v1/state_codec.h`
- `smartledz_protocol/v1/color_math.h`

## Notes

- Pure C++ (STL only).
- Implements command payload builders, notify payload application, and CT/RGB conversion helpers.

## Doxygen

Generate API docs locally:

```bash
(cd docs && doxygen Doxyfile)
```

Open:

- `docs/.doxygen/html/index.html`

Generated documentation output under `docs/.doxygen/` is excluded from version control.

Docs are published with GitHub Pages via:

- `.github/workflows/deploy-pages.yml`

Set repository Pages source to `GitHub Actions` in repository settings.
