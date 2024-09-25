# roomba_basic
ルンバ制御用の基本コマンド集
C++/python言語で記述．
Windows MinGW/Linux GCCに対応.

コマンドラインで動く必要最低限のもの．

Lectureブランチ→講習会用にさらにシンプルに

# C++開発環境
Windows +MinGW環境だと，
ポータブルVSCode + python-embeddable + minGW64 環境を想定．<br>
ESA <br>
https://doilab.esa.io/posts/606
<br>
<s>出村先生の環境を想定（C:\cprog20以下にコンパイラ一式配置）
https://demura.net/education/17784.html
</s>

# pythonファイル群（pythonディレクトリ）
roomba_basic.py → テスト用プログラム．機能限定版

roomba_def.py → 定数，関数定義

# C++ファイル群（c_cppディレクトリ）
Makefile → コンパイル設定ファイル．コンパイラのパス，ソースファイル群等を指定．

roomba_test.cpp →　メインプログラム

roomba_cmd.h →　よく使う関数など

roomba_types.h　→　データ構造定義．オドメトリなど若干高度な用途

serial.cpp,h →　シリアル通信関連



