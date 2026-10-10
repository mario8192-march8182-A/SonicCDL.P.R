# Sonic CD: Little Planet Revisited
# Prototype Audio & Modding Guide

## 1. Objetivo

Este documento define como o projeto suporta:
- protótipos musicais do Sonic CD
- sons alternativos e remixes
- mods e conteúdo do usuário
- carregamento de recursos fora do conteúdo oficial

A ideia é que o engine carregue conteúdo do diretório `Mods/` e também aceite arquivos de protótipo em formatos modernos como OGG/WAV/PNG.

## 2. Estrutura de Mod

```
Mods/
  MyMod/
    mod.json
    Audio/
      Prototype/
        proto_title.ogg
      Music/
        stage_1.ogg
      SFX/
        jump.wav
    Sprites/
      Sonic/
    Levels/
      stage_1.sonlvl
    Scripts/
      init.lua
```

## 3. Regras de Mod

- A pasta `Mods/` não deve incluir ativos oficiais da SEGA.
- Cada mod precisa de um `mod.json`.
- O usuario deve fornecer os seus próprios assets e nomes.
- Mods podem sobrescrever recursos existentes sem alterar os arquivos originais.

## 4. Formato de Manifesto de Audio

A engine aceita manifests simples para prototipos musicais:

```
id=proto_title
name=Prototype Title Theme
file=Audio/Prototype/proto_title.ogg
original_game=Sonic CD
region=US
is_prototype=true
has_loop=true
notes=Tema antigo usado em build inicial
---
```

Esses manifests podem ser lidos pelo `PrototypeAudioBank`.

## 5. Protótipos de Jogos

Protótipos são particularmente valiosos porque permitem:
- explorar temas não usados
- testar ideias de áudio e gameplay
- compatibilizar remixes com a estética do fangame
- criar versões alternativas sem quebrar o jogo principal

A engine deve tratar protótipos como conteúdo opcional, nunca como substituição obrigatória do jogo base.

## 6. SonLvL Editor

Para fases e tilemaps, a estrutura pode seguir o padrão de SonLvL:

- `Levels/` usa arquivos `.sonlvl` ou `.json` compatíveis com o editor
- cada nível precisa de:
  - layout/tiles
  - metadata do palco
  - lista de objetos
  - paleta vinculada
  - música associada

A referência tecnológica para o editor pode ser:
- arquivo de tilemap
- arquivo de metadata do nível
- arquivo de objetos
- arquivo de palette map

## 7. Personagens jogáveis extras

Personagens adicionais devem seguir a mesma estrutura de `mod.json`, por exemplo:

- `Characters/Amy`
- `Characters/Knuckles`
- `Characters/Tails`
- `Characters/Gemstone Sonic`

Cada personagem precisa de:
- sprites por estado
- paleta
- física e hitbox
- script de habilidade

## 8. Recomendação de uso

- Prototipos de áudio devem viver em `Audio/Prototype/`
- Sons de personagem em `Audio/SFX/`
- Musicas de fase em `Audio/Music/`
- Arquivos de nível em `Levels/`
- Scripts em `Scripts/`

## 9. Meta da Engine

O projeto deve respeitar as regras:
- sem ativos protegidos
- sem redistribuir conteúdo da SEGA
- o usuário fornece seu próprio material
- mods só funcionam com conteúdo licenciado ou autoral do usuário

## 10. Próximo passo

O próximo passo técnico recomendável é:
- criar o `Audio/PrototypeAudioBank`
- salvar manifests em `Mods/`
- suportar `mod.json` e `prototype_audio.txt` como formatos de entrada
- integrar uma carregadora de música para `.ogg` e `.wav`
