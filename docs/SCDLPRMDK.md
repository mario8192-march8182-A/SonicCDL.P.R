# SCDLPRMDK - Sonic CD: Little Planet Revisited Mod Development Kit

## Overview

O **SCDLPRMDK** é o kit oficial de desenvolvimento de mods para o Sonic CD: Little Planet Revisited. Este documento fornece tudo o que você precisa para criar mods, personagens, fases, sons e conteúdo para o fangame.

## Estrutura de Mod Básica

Todo mod deve estar em uma pasta dentro de `Mods/` e seguir essa estrutura:

```
Mods/
  MeuMod/
    mod.json
    Audio/
      Music/
      SFX/
      Prototype/
    Sprites/
      Characters/
      Objects/
      UI/
    Levels/
    Scripts/
    Data/
    README.md
```

## mod.json - Manifest do Mod

Cada mod **DEVE** ter um arquivo `mod.json` na raiz da pasta:

```json
{
  "name": "Nome do Meu Mod",
  "author": "Seu Nome",
  "version": "1.0.0",
  "description": "Descrição breve do mod",
  "tags": ["characters", "levels", "audio"],
  "enabled": true,
  "assets": {
    "character_knuckles": "Sprites/Characters/knuckles.png",
    "level_1_music": "Audio/Music/custom_level_1.ogg",
    "proto_title": "Audio/Prototype/proto_title_002.ogg"
  },
  "scripts": [
    "Scripts/init.lua",
    "Scripts/characters.lua"
  ]
}
```

### Campos Obrigatórios:
- `name` - nome do mod
- `author` - criador do mod
- `version` - versão semântica (MAJOR.MINOR.PATCH)
- `description` - descrição do conteúdo

### Campos Opcionais:
- `tags` - categorias (characters, levels, audio, ui, etc)
- `enabled` - ativa/desativa o mod sem removê-lo
- `assets` - mapeamento de chave → arquivo
- `scripts` - scripts LUA a executar

## Personagens Jogáveis

### Estrutura de Personagem

Cada personagem precisa de:

```
Characters/
  knuckles/
    manifest.json
    sprites.png
    palette.txt
    animations.json
    physics.json
```

### character_manifest.json

```json
{
  "id": "knuckles",
  "display_name": "Knuckles",
  "author": "ALTGR109",
  "description": "Knuckles the Echidna - Um lutador poderoso",
  "base_speed": 440,
  "max_speed": 520,
  "acceleration": 1800,
  "jump_strength": 620,
  "abilities": [
    "climb_walls",
    "glide",
    "punch"
  ],
  "sprite_sheet": "sprites.png",
  "palette": "palette.txt",
  "animations": "animations.json"
}
```

## Fases/Níveis

### Estrutura de Fase

```
Levels/
  stage_1_boss/
    level.json
    tileset.png
    palette.txt
    layout.bin
    objects.json
    music.ogg
```

### level.json

```json
{
  "id": "stage_1_boss",
  "name": "Boss Arena",
  "act": 1,
  "music": "music.ogg",
  "tileset": "tileset.png",
  "palette": "palette.txt",
  "layout_width": 256,
  "layout_height": 192,
  "objects": "objects.json",
  "time_limit": 300,
  "rings_bonus_threshold": 150
}
```

## Protótipos de Áudio

### Estrutura de Áudio

```
Audio/
  Music/
    stage_remix.ogg
  SFX/
    jump_alt.wav
  Prototype/
    proto_title_002.ogg
    proto_speed_shoes_002.ogg
```

### Audio Manifest

```
id=proto_title_002
name=Prototype Title Theme (0.02)
file=Audio/Prototype/proto_title_002.ogg
original_game=Sonic CD
region=US
is_prototype=true
has_loop=true
composer=Naoki Yamaguchi
notes=Tema antigo do protótipo 0.02 - Sonic CD
---
id=proto_speed_shoes_002
name=Prototype Speed Shoes (0.02)
file=Audio/Prototype/proto_speed_shoes_002.ogg
original_game=Sonic CD
region=US
is_prototype=true
has_loop=true
notes=Música de Speed Shoes do protótipo 0.02
---
```

## Scripts LUA

Mods podem incluir scripts LUA para lógica customizada:

```lua
-- Scripts/init.lua
function OnModLoad()
    print("Mod carregado!")
    RegisterCharacter("knuckles", CHARACTER_DATA)
end

function OnGameStart()
    -- Lógica de início de jogo
end

function OnLevelLoad(levelId)
    -- Lógica de carregamento de fase
end
```

## Gemstone Sonic - Forma Exclusiva

### Transformação em Gemstone Sonic

```json
{
  "id": "gemstone_sonic",
  "display_name": "Gemstone Sonic",
  "base_form": "sonic",
  "transformation_item": "time_stones",
  "required_count": 7,
  "sprite_sheet": "gemstone_sonic.png",
  "abilities": [
    "temporal_invulnerability",
    "crystal_barrier",
    "enhanced_speed",
    "crystalline_jump"
  ],
  "speed_multiplier": 1.5,
  "duration": 30,
  "music": "gemstone_transformation.ogg"
}
```

## Padrões de Nomeação

- Arquivos de mod: `snake_case` (ex: `my_mod`, `character_knuckles`)
- IDs internos: `snake_case` (ex: `stage_1_boss`, `proto_title_002`)
- Nomes de arquivo de imagem: `snake_case.png`
- Arquivos de áudio: `snake_case.ogg` ou `snake_case.wav`

## Boas Práticas

1. **Sempre use `mod.json`** - sem ele, o mod não será detectado
2. **Documente seu mod** - inclua um `README.md` com instruções
3. **Caminhos relativos** - use caminhos relativos à pasta do mod
4. **Sem dependências externas** - o mod deve ser auto-contido
5. **Teste compatibilidade** - certifique-se que o mod funciona com a engine
6. **Respeite licenças** - não distribua conteúdo protegido

## Checklist para Publicação

- [ ] `mod.json` presente e válido
- [ ] Arquivos de asset referenciados existem
- [ ] `README.md` com instruções de instalação
- [ ] Todos os scripts testados
- [ ] Sem conteúdo protegido por direitos autorais
- [ ] Versionamento semântico correto
- [ ] Tags apropriadas

## Suporte e Comunidade

- **Discussions**: GitHub Discussions do projeto
- **Issues**: Reporte bugs e solicite recursos
- **Exemplos**: Veja mods de exemplo em `Mods/Examples/`

## Próximos Passos

1. Escolha o tipo de mod que quer fazer (character, level, audio)
2. Crie a pasta `Mods/MeuMod/`
3. Copie o `mod.json` de exemplo e customize
4. Adicione seus arquivos de asset
5. Teste com a engine
6. Publique e compartilhe!

---

**Versão SCDLPRMDK**: 1.0.0
**Última atualização**: 2026-10-11
