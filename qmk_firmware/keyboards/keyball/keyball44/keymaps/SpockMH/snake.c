// 設定
#define SNAKE_LENGTH 5                // ヘビの長さ (セグメント数)。5セグメント x 2ピクセル = 長さ10ピクセル
#define SNAKE_BLOCK_SIZE 2            // 1セグメントのピクセルサイズ (幅2ピクセル)
#define OLED_HEIGHT 128
#define OLED_WIDTH 32

// グリッドサイズの計算 (座標管理をブロック単位にするため)
#define SNAKE_GRID_WIDTH (OLED_WIDTH / SNAKE_BLOCK_SIZE)
#define SNAKE_GRID_HEIGHT (OLED_HEIGHT / SNAKE_BLOCK_SIZE)

// データ構造と変数
typedef struct { uint8_t x; uint8_t y; } point_t;
typedef enum { UP, DOWN, LEFT, RIGHT } direction_t;

//tatic bool is_snake_active = false;
static point_t snake_body[SNAKE_LENGTH];
static direction_t snake_direction;

// 初期化処理
void snake_init_light(void) {
    snake_direction = RIGHT;
    // 画面中央からヘビを開始 (座標はピクセルではなくブロック単位で管理)
    for (uint8_t i = 0; i < SNAKE_LENGTH; ++i) {
        snake_body[i] = (point_t){ .x = (SNAKE_GRID_WIDTH / 2) - i, .y = (SNAKE_GRID_HEIGHT / 2) };
    }
}

// 更新処理
void snake_update_light(void) {
    // 体を1セグメントずつずらす
    for (uint8_t i = SNAKE_LENGTH - 1; i > 0; --i) {
        snake_body[i] = snake_body[i - 1];
    }
    // 新しい頭の位置を計算 (ブロック単位)
    point_t* head = &snake_body[0];
    switch (snake_direction) {
        case UP:    head->y--; break;
        case DOWN:  head->y++; break;
        case LEFT:  head->x--; break;
        case RIGHT: head->x++; break;
    }
    // 壁に当たったら反対側から出現させる（ループ処理）
    head->x %= SNAKE_GRID_WIDTH;
    head->y %= SNAKE_GRID_HEIGHT;
}

// 描画処理
void render_snake_light(void) {
    oled_clear();
    // 各セグメントを2x2ピクセルのブロックとして描画
    for (uint8_t i = 0; i < SNAKE_LENGTH; ++i) {
        for (uint8_t px = 0; px < SNAKE_BLOCK_SIZE; ++px) {
            for (uint8_t py = 0; py < SNAKE_BLOCK_SIZE; ++py) {
                oled_write_pixel((snake_body[i].x * SNAKE_BLOCK_SIZE) + px, (snake_body[i].y * SNAKE_BLOCK_SIZE) + py, true);
            }
        }
    }
}


// QMKで呼ばれるユーザー描画関数の実装（ここで粒を降らせてる）
void oledkit_render_info_user(void) {
    render_snake_light();
}
