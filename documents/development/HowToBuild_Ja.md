﻿# ビルド手順

## 必須

### 共通

- cmake(3.15以上)
- git
- python3.x

### Windows

- VisualStudio(2019以降)

### Linux

- gcc

#### Ubuntu

```
libudev-dev
libssl-dev # to save and load a package with a password
```

## サブモジュール取得

スクリプト `scripts/Pull.bat` を実行する。

## プロジェクト生成


下記のスクリプトを実行します。

- Windows の場合： `scripts/GenerateProjects.bat`
- macOS の場合： `scripts/GenerateProjects_Mac.sh`
- Linux の場合： `scripts/GenerateProjects_Linux.sh`

## Wrapper生成

スクリプト `scripts/generate_wrapper.py` を実行します。

## ビルド

### Windows

`build/Altseed.sln` を開き ソリューションごとビルドします。 `Altseed_Core_Test` を実行し、テストが走ったら成功です。