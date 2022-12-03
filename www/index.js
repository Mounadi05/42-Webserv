const cnv = document.getElementById("canvas");
const ctx = cnv.getContext("2d");
const left = document.querySelector(".left");
const right = document.querySelector(".right");
const startGame = document.querySelector(".start-game");
const status = document.querySelector(".start-game h1");

const cw = cnv.width;
const ch = cnv.height;

ctx.fillStyle = "#000";
ctx.fillRect(0, 0, cw, ch);

let vel = {
  x: 6,
  y: 6,
};

let dir;
let leftDir, rightDir;
let Enemies = [];

class Ball {
  constructor(x, y, raduis) {
    this.x = x;
    this.y = y;
    this.raduis = raduis;
  }

  draw() {
    ctx.beginPath();
    ctx.arc(this.x, this.y, this.raduis, 0, 2 * Math.PI);
    ctx.fillStyle = "#fff";
    ctx.fill();
  }

  update() {
    this.draw();
    this.x += vel.x;
    this.y -= vel.y;
    if (this.x + this.raduis > cw || this.x - this.raduis < 0) {
      vel.x = -vel.x;
    }

    if (this.y - this.raduis < 0) {
      vel.y = -vel.y;
    }
  }
}

class Pipe {
  constructor(x, y, w, h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
  }

  draw() {
    ctx.fillStyle = "#fff";
    ctx.fillRect(this.x, this.y, this.w, this.h);
  }

  update() {
    this.draw();
    if (leftDir && this.x > 0) {
      this.x -= 7;
    }

    if (rightDir && this.x + this.w < cw) {
      this.x += 7;
    }

    if (collision(this)) {
      vel.y = -vel.y;
    }
  }
}

class Enemy {
  constructor(x, y, w, h, color) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.color = color;
  }

  draw() {
    ctx.fillStyle = this.color;
    ctx.fillRect(this.x, this.y, this.w, this.h);
  }

  update() {
    this.draw();
    if (collision(this)) {
      vel.y = -vel.y;
    }
  }
}

const ball = new Ball(cw / 2, 354, 8);
const pipe = new Pipe(cw / 2 - 30, 364, 60, 10);

function animate() {
  ctx.fillStyle = "#000";
  ctx.fillRect(0, 0, cw, ch);
  requestId = requestAnimationFrame(animate);

  Enemies.forEach((enemy, index) => {
    if (collision(enemy)) {
      setTimeout(() => {
        switch (enemy.color) {
          case "#fff":
            enemy.color = "blue";
            break;
          case "blue":
            enemy.color = "red";
            break;
          case "red":
            Enemies.splice(index, 1);
        }
      }, 0);
    }
    enemy.update();
  });

  if (ball.y > ch || Enemies.length < 1) {
    cancelAnimationFrame(requestId);
    startGame.style.display = "block";
    if (ball.y > ch) {
      status.innerText = "You Lose";
    } else {
      status.innerText = "You Win";
    }
  }

  if (player == "boot") {
    boot();
  }

  // boot track posistion of ball and move pipe

  ball.update();
  pipe.update();
}

function collision(rect) {
  distX = Math.abs(ball.x - rect.x - rect.w / 2);
  distY = Math.abs(ball.y - rect.y - rect.h / 2);

  if (distX > rect.w / 2 + ball.raduis) {
    return false;
  }
  if (distY > rect.h / 2 + ball.raduis) {
    return false;
  }

  if (distX <= rect.w / 2) {
    return true;
  }
  if (distY <= rect.h / 2) {
    return true;
  }
  const dx = distX - rect.w / 2;
  const dy = distY - rect.h / 2;
  return dx * dx + dy * dy <= ball.raduis * ball.raduis;
}

function createEnemies() {
  Enemies = [];
  for (let y = 0; y < 5; y++) {
    for (let x = 0; x < 6; x++) {
      Enemies.push(new Enemy(x * 60 + 15, y * 25 + 15, 50, 15, "#fff"));
    }
  }
}

function boot() {
  if (ball.y >= 150) {
    if (vel.y < 0) {
      if (pipe.x + 30 - ball.x < -7 && pipe.x + pipe.w < cw) {
        pipe.x += 7;
      }
      if (pipe.x + 30 - ball.x > 7 && pipe.x > 0) {
        pipe.x -= 7;
      }
    }
  }

  if (vel.y > 0) {
    if (pipe.x + 30 > cw / 2) {
      pipe.x -= 5;
    }
    if (pipe.x + 30 < cw / 2) {
      pipe.x += 5;
    }
  }
}

startGame.addEventListener("click", (e) => {
  if (e.target.id == "boot") {
    player = "boot";
  } else {
    player = "player";
  }
  startGame.style.display = "none";

  ball.x = cw / 2;
  ball.y = 354;
  pipe.x = cw / 2 - 30;
  pipe.y = 364;

  createEnemies();
  animate();
});

function addListenerMulti(element, eventNames, listener) {
  var events = eventNames.split(" ");
  for (var i = 0, iLen = events.length; i < iLen; i++) {
    element.addEventListener(events[i], listener, false);
  }
}

addListenerMulti(document, "touchstart mousedown keydown", (e) => {
  if (e.target == left || e.keyCode == "37") {
    leftDir = true;
  } else if (e.target == right || e.keyCode == "39") {
    rightDir = true;
  }
});
addListenerMulti(document, "touchend mouseup keyup", () => {
  leftDir = rightDir = false;
});

createEnemies();
