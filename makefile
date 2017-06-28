MorseCodeTables.exe : MorseCodeTables.c
	gcc MorseCodeTables.c -o MorseCodeTables.exe
	MorseCodeTables.exe

MorseTable.h : MorseTones.txt MorseCodeTables.c
	MorseCodeTables.exe

MCP.exe : MCP.c MorseTable.h
	gcc MCP.c -o MCP.exe

BuildMcpExersizes.exe: BuildMcpExersizes.c
	gcc BuildMcpExersizes.c -o BuildMcpExersizes.exe

G1mcp.txt: BuildMcpExersizes.exe mcp.txt
	BuildMcpExersizes.exe 50ETAR mcp.txt G1mcp.txt

G2mcp.txt: BuildMcpExersizes.exe mcp.txt
	BuildMcpExersizes.exe 50ETARSLUQJ mcp.txt G2mcp.txt

G3mcp.txt: BuildMcpExersizes.exe mcp.txt
	BuildMcpExersizes.exe 50ETARSLUQJHONCV mcp.txt G3mcp.txt

G4mcp.txt: BuildMcpExersizes.exe mcp.txt
	BuildMcpExersizes.exe 50ETARSLUQJHONCVIBYP mcp.txt G4mcp.txt

G5mcp.txt: BuildMcpExersizes.exe mcp.txt
	BuildMcpExersizes.exe 50ETARSLUQJHONCVIBYPWKZM mcp.txt G5mcp.txt

mcpexergen.exe: mcpexergen.c
	gcc mcpexergen.c -o mcpexergen.exe

SignedMagic.exe: SignedMagic.cpp
	g++ SignedMagic.cpp -o SignedMagic.exe

UnSignedMagic.exe: UnSignedMagic.cpp
	g++ UnSignedMagic.cpp -o UnSignedMagic.exe

SignedMagic16.exe: SignedMagic16.cpp
	g++ SignedMagic16.cpp -o SignedMagic16.exe

UnSignedMagic16.exe: UnSignedMagic16.cpp
	g++ UnSignedMagic16.cpp -o UnSignedMagic16.exe

SignedMagic8.exe: SignedMagic8.cpp
	g++ SignedMagic8.cpp -o SignedMagic8.exe

UnSignedMagic8.exe: UnSignedMagic8.cpp
	g++ UnSignedMagic8.cpp -o UnSignedMagic8.exe
