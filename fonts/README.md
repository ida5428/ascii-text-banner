## Fonts
Currently, there is only one additional (incomplete) font, besides to the built-in default one.

External fonts can be listed through the ``list-fonts`` subcommand:
```bash
./build/main list-fonts
```

To create a custom font, start by editing the `template.toml` file [here](template.toml).
- You only need to define the characters you want, any missing characters will automatically fallback to the default font.
- Once the font file is ready, use the ``-f`` or ``--font`` flag when running the program:
```bash
./build/main -i "Hello!" -f <your-font-name>
```

