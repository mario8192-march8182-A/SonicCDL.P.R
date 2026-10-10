# Sonic CD: Little Planet Revisited
# Modding and Prototype Audio Documentation

Este documento detalha o suporte do projeto a mods, protótipos e conteúdo externo.

## Mod Structure

```
Mods/
  NomeDoMod/
    mod.json
    Audio/
      Prototype/
      Music/
      SFX/
    Sprites/
    Levels/
    Scripts/
```

## Manifest Example

```json
{
  "name": "Prototype SFX Pack",
  "author": "ALTGR109",
  "version": "1.0.0",
  "description": "Additional prototype audio and remixes",
  "audio": {
    "prototype_tracks": [
      "Audio/Prototype/proto_title.ogg"
    ]
  }
}
```

## SonLvL Compatibility

A engine pode carregar fases em formato equivalente ao SonLvL mediante:
- metadata do level
- tileset
- palette
- objetos
- scripts de spawn

Os arquivos devem ficar em `Levels/` e ser referenciados no `mod.json`.

## Prototype Audio Policy

- protótipos não são substitutos do jogo base
- devem ser carregados como conteúdo opcional
- podem ser ativados por mod ou menu de opções
- devem manter a estética do fangame

## Recommended Standards

- use `.ogg` para música
- use `.wav` para efeitos de som curtos
- use `.json` para metadata de mod
- use `.sonlvl` ou `.json` para fases
- use `.lua` ou `.json` para scripts

## Next Development Steps

1. implement `mod.json` parser
2. implement `PrototypeAudioBank`
3. implement audio library integration with SDL3_mixer
4. add UI for mod selection and prototype themes
