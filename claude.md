# CLAUDE.md

Repository guidance for AI agents working on `ESP-IDF-FastLED`.

## Project Scope

- This repo packages FastLED for native ESP-IDF usage.
- `src/` is vendored from upstream FastLED and should stay as close to upstream as possible.
- Prefer integration fixes in top-level component files and docs before patching `src/`.

## Source of Truth

- Upstream FastLED: `https://github.com/FastLED/FastLED`
- Keep track of upstream commit used for sync in `README.md`.
- Use `scripts/sync_from_upstream.ps1` for source refresh.

## Preferred Change Strategy

1. Avoid broad rewrites in `src/` unless required for ESP-IDF compatibility.
2. Keep ESP-IDF-specific behavior in:
   - `CMakeLists.txt`
   - `idf_component.yml`
   - `docs/`
3. If modifying vendored FastLED internals, document:
   - Why change is needed
   - Upstream file path
   - Whether it should be upstreamed

## Validation Expectations

- Verify the component builds in an ESP-IDF project (`idf.py build`) before release.
- Validate at least one real strip on hardware for timing/smoothness-sensitive changes.

## Documentation Requirements

For any behavior-affecting change, update:

- `README.md` (user-visible behavior)
- `docs/ESP_IDF_USAGE.md` (integration and tuning)
