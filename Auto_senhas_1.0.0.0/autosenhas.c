#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Variável global para armazenar o tamanho da senha
int pass_lenght;

// Buffer para armazenar a senha gerada
char asdqwd[100];

// Definição da estrutura Button para criar botões na interface
typedef struct Button {
    Rectangle bounds; // Área do botão
    const char *text; // Texto do botão
    Color color; // Cor padrão do botão
    Color hoverColor; // Cor do botão quando o mouse está sobre ele
    Color pressedColor; // Cor do botão quando está sendo pressionado
    Texture2D texture;
    Texture2D hoverTexture;
    Texture2D pressedTexture;
} Button;

// Função para desenhar um botão na tela
void DrawButton(Button button) {
    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, button.bounds);
    bool isPressed = isHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (isPressed) {
        DrawRectangleRec(button.bounds, button.pressedColor);
        DrawTexture(button.pressedTexture, button.bounds.x, button.bounds.y, WHITE);
    } else if (isHovered) {
        DrawRectangleRec(button.bounds, button.hoverColor);
        DrawTexture(button.hoverTexture, button.bounds.x, button.bounds.y, WHITE);
    } else {
        DrawRectangleRec(button.bounds, button.color);
        DrawTexture(button.texture, button.bounds.x, button.bounds.y, WHITE);
    }

    int textWidth = MeasureText(button.text, 20);
    int textHeight = 20; // Assumindo tamanho de fonte 20
    DrawText(button.text,
             button.bounds.x + (button.bounds.width - textWidth) / 2,
             button.bounds.y + (button.bounds.height - textHeight) / 2,
             20,
             BLACK);
}

// Função para verificar se um botão foi clicado
bool IsButtonClicked(Button button) {
    Vector2 mousePoint = GetMousePosition();
    return CheckCollisionPointRec(mousePoint, button.bounds) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

// Função para gerar uma senha aleatória
void gerarSenha(char *senha) {
    const char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int tamanhosenha = pass_lenght;
    
    // Inicializa o gerador de números aleatórios
    srand(time(0));

    for (int i = 0; i < tamanhosenha; ++i) {
        senha[i] = caracteres[rand() % (sizeof(caracteres) - 1)];
    }
    senha[tamanhosenha] = '\0';  // Adiciona o caractere nulo no final da string
}

// Função principal do programa
int main(void) {
    // Inicialização
    const int screenWidth = 800;
    const int screenHeight = 450;
    char senha[100];

    InitWindow(screenWidth, screenHeight, "AutoSenhas v1.0");

    const int minChars = 6;
    const int maxChars = 12;
    int charCount = minChars;

    Texture2D buttonTexture = LoadTexture("info_icon.png");
    Texture2D hoverTexture = LoadTexture("info_icon_hover.png");
    Texture2D pressedTexture = LoadTexture("info_icon_click.png");

    // Carrega um ícone para a janela
    Texture2D icon = LoadTexture("key.png");
    Image Window_icon = LoadImageFromTexture(icon);
    SetWindowIcon(Window_icon);

    // Inicializa o botão "copy"
    Button myButton = {
        .bounds = { screenWidth / 2 , screenHeight / 2 + 110, 100, 40 },
        .text = "copy",
        .color = LIGHTGRAY,
        .hoverColor = GRAY,
        .pressedColor = DARKPURPLE,
    };

    Button myButton_img = {
        .bounds = { 10, 10, 100, 40 },
        .text = "",
        .texture = buttonTexture,
        .hoverTexture = hoverTexture,
        .pressedTexture = pressedTexture
    };
    
    // Definição dos retângulos para os botões de aumentar e diminuir o tamanho da senha
    Rectangle btnIncrease = { 350, 200, 50, 50 };
    Rectangle btnDecrease = { 250, 200, 50, 50 };

    SetTargetFPS(60); // Define o jogo para rodar a 60 quadros por segundo

    // Loop principal do jogo
    while (!WindowShouldClose()) { // Detecta o fechamento da janela ou tecla ESC
        // Atualização
        if (CheckCollisionPointRec(GetMousePosition(), btnIncrease) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (charCount < maxChars) charCount++;
            pass_lenght = charCount;
            gerarSenha(senha);
        }
        if (CheckCollisionPointRec(GetMousePosition(), btnDecrease) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (charCount > minChars) charCount--;
            pass_lenght = charCount;
            gerarSenha(senha);
        }

        if (IsButtonClicked(myButton)) {
            // Define a ação ao clicar no botão "copy"
            SetClipboardText(senha);
        }

        if (IsButtonClicked(myButton_img)) {
            // Define a ação ao clicar no botão "copy"
            OpenURL("https://www.instagram.com/gviz_studio/");  
        }



        // Desenho
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Ajustar quantidade de caracteres", 200, 150, 20, DARKGRAY);

            // Desenha o botão de aumentar
            DrawRectangleRec(btnIncrease, LIGHTGRAY);
            DrawRectangleLinesEx(btnIncrease, 2, DARKGRAY);
            DrawText("+", btnIncrease.x + 15, btnIncrease.y + 10, 30, BLACK);

            // Desenha o botão de diminuir
            DrawRectangleRec(btnDecrease, LIGHTGRAY);
            DrawRectangleLinesEx(btnDecrease, 2, DARKGRAY);
            DrawText("-", btnDecrease.x + 15, btnDecrease.y + 10, 30, BLACK);

            // Mostra a quantidade de caracteres selecionada e a senha gerada
            DrawText(TextFormat("Quantidade de caracteres: %d", charCount), 200, 300, 20, BLACK);
            DrawText(senha, 200, 350, 20, BLACK);

            // Desenha o botão "copy"
            DrawButton(myButton);
            DrawButton(myButton_img);

        EndDrawing();
    }

    // Desinicialização
    CloseWindow(); // Fecha a janela e o contexto OpenGL

    return 0;
}
