<div align="center">

# RealmWeaver

[<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" width="60">](https://isocpp.org)
[<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cmake/cmake-original.svg" width="60">](https://cmake.org)

[![Open in Visual Studio Code](https://img.shields.io/badge/Open%20in%20VS%20Code-007ACC?logo=visual-studio-code&logoColor=white)](https://vscode.dev/)
[![Contributors](https://img.shields.io/github/contributors/lakshyajain-0291/RealmWeaver)](https://github.com/lakshyajain-0291/RealmWeaver/graphs/contributors)
[![Forks](https://img.shields.io/github/forks/lakshyajain-0291/RealmWeaver?style=social)](https://github.com/lakshyajain-0291/RealmWeaver/network/members)
[![Stars](https://img.shields.io/github/stars/lakshyajain-0291/RealmWeaver?style=social)](https://github.com/lakshyajain-0291/RealmWeaver/stargazers)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/lakshyajain-0291/RealmWeaver)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://github.com/lakshyajain-0291/RealmWeaver/blob/main/LICENSE)

*A Dynamic 2D Open-Ended Game with AI-Generated Content*

[Features](#key-features) • [Installation](#installation) • [Commands](#commands) • [Contributing](#contribution)

</div>

## 🌟 Overview

**RealmWeaver** is a cutting-edge 2D open-ended game that revolutionizes interactive storytelling through dynamic content generation. Leveraging the GEMINI API, the game creates unique NPCs, quests, locations, and items, ensuring no two gameplay experiences are alike.

## 🚀 Key Features

- 🧙 **Dynamic Content Generation**: AI-powered world creation using GEMINI API
- 🏆 **Rank-Based System**: Adaptive difficulty with NPCs, quests, and items ranked 1-10
- 🗺️ **Interconnected World Map**: Complex location relationships and pathfinding
- 🎮 **Interactive Command System**: Intuitive commands for seamless gameplay
- 🧩 **DSA-Based Minigames**: Challenging, adaptive mini-challenges
- ⚙️ **Flexible Configuration**: Easy game customization through config files

## 🌈 Why RealmWeaver?

- **Unique Experience**: Every playthrough is a new adventure
- **Intelligent Design**: Smart algorithms drive game mechanics
- **Flexibility**: Easily extendable and configurable
- **Educational**: Demonstrates advanced data structures and algorithms
- **Immersive**: AI-generated content creates rich, dynamic narratives

## 📋 Prerequisites

- C++ compiler (C++11 or higher)
- CMake (3.1 or higher)
- GEMINI API Key
- Git (optional)

## 🔧 Installation

<details>
<summary>Step-by-step guide</summary>

1. Clone the repository:
```bash
git clone https://github.com/lakshyajain-0291/RealmWeaver.git
cd RealmWeaver
```

2. Set up GEMINI API Key:
```bash
# Create .env file
echo "GEMINI_API_KEY=YOUR_API_KEY" > .env
```

3. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

4. Run the game:
```bash
./RealmWeaver
```
</details>

## 🎮 Available Commands

- `npcs`: List NPCs at current location
- `modifyconfig`: Change game configuration
- `exit`: End game session
- `status`: View player stats
- `inventory`: Check item collection
- `help`: Show available commands
- `quest`: Manage quests
- `interact`: Engage with NPCs
- `move`: Travel between locations
- And more!

## 🔑 Example Workflow

1. Start the game
2. Use `help` to explore commands
3. `move` to different locations
4. `interact` with NPCs
5. Accept and complete quests
6. Manage your `inventory`
7. Customize game with `modifyconfig`

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/NewGameplay`)
3. Commit your changes (`git commit -m 'Add new dynamic quest system'`)
4. Push to the branch (`git push origin feature/NewGameplay`)
5. Open a Pull Request

## 📜 License

RealmWeaver is open-source, released under the MIT License. See `LICENSE` for details.

## 🙏 Acknowledgments

- GEMINI API for dynamic content generation
- Data Structures and Algorithms course for inspiration
- Open-source community

**Happy adventuring!** 🎲🌍
