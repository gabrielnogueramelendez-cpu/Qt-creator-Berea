import Reveal from 'reveal.js';
import Notes from 'reveal.js/plugin/notes/notes.esm.js';
import { gsap } from 'gsap';
import { MotionPathPlugin } from 'gsap/MotionPathPlugin';

gsap.registerPlugin(MotionPathPlugin);

import 'reveal.js/dist/reveal.css';
import '@fontsource-variable/bricolage-grotesque/wdth.css';
import '@fontsource/atkinson-hyperlegible/400.css';
import '@fontsource/atkinson-hyperlegible/700.css';
import './style.css';

const reducir = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
const ALFABETO = 'ABCDEFGHJKLMNPQRSTUVWXYZ0123456789';

/* ---------- visor de código: arma los 5 casilleros ---------- */
document.querySelectorAll('.flap[data-codigo]').forEach((flap) => {
  flap.innerHTML = [...flap.dataset.codigo]
    .map((ch) => `<span class="c" aria-hidden="true">${ch}</span>`)
    .join('');
});

function girarVisor(flap) {
  if (reducir || !flap) return;
  const final = flap.dataset.codigo;
  flap.querySelectorAll('.c').forEach((c, i) => {
    let vueltas = 6 + i * 3;
    const tick = () => {
      if (vueltas-- <= 0) { c.textContent = final[i]; gsap.fromTo(c, { rotationX: -70 }, { rotationX: 0, duration: 0.22, ease: 'back.out(2)' }); return; }
      c.textContent = ALFABETO[Math.floor(Math.random() * ALFABETO.length)];
      gsap.fromTo(c, { rotationX: -60 }, { rotationX: 0, duration: 0.07, ease: 'none', onComplete: tick });
    };
    tick();
  });
}

/* ---------- contadores ---------- */
function contar(slide) {
  slide.querySelectorAll('[data-contar]').forEach((el) => {
    const fin = Number(el.dataset.contar);
    if (reducir) { el.textContent = fin; return; }
    const obj = { v: 0 };
    gsap.to(obj, { v: fin, duration: 1.2, ease: 'power2.out', onUpdate: () => { el.textContent = Math.round(obj.v); } });
  });
  if (!reducir) {
    slide.querySelectorAll('.lr-fila').forEach((b) => gsap.from(b, { width: 0, duration: 1.6, ease: 'power2.inOut', delay: 0.2 }));
  }
}

/* ---------- pestañas con reproducción automática ---------- */
const temporizadores = new Map();

function pestanias(slide, selBoton, selPanel, intervalo) {
  const botones = [...slide.querySelectorAll(selBoton)];
  const paneles = [...slide.querySelectorAll(selPanel)];
  const play = slide.querySelector('[data-play]');
  const mostrar = (i) => {
    botones.forEach((b, j) => { b.classList.toggle('activa', i === j); b.setAttribute('aria-selected', String(i === j)); });
    paneles.forEach((p, j) => p.classList.toggle('activa', i === j));
    if (!reducir) gsap.from(paneles[i], { opacity: 0, y: 12, duration: 0.35 });
  };
  const parar = () => { clearInterval(temporizadores.get(slide)); temporizadores.delete(slide); play?.setAttribute('aria-pressed', 'false'); };
  botones.forEach((b, i) => b.addEventListener('click', () => { parar(); mostrar(i); }));
  play?.addEventListener('click', () => {
    if (temporizadores.has(slide)) { parar(); return; }
    play.setAttribute('aria-pressed', 'true');
    let i = botones.findIndex((b) => b.classList.contains('activa'));
    temporizadores.set(slide, setInterval(() => { i = (i + 1) % botones.length; mostrar(i); }, intervalo));
  });
  slide._parar = parar;
}

pestanias(document.getElementById('arbol-problemas'), '.causa', '.raiz-grupo', 2600);
pestanias(document.getElementById('recorrido'), '.rec-paso', '.rec-fig', 3200);

/* ---------- carrera en el mostrador (tiempos proporcionales: 30 s frente a 7 min) ---------- */
function carrera(slide) {
  const chip = slide.querySelector('#chip-estado');
  chip.textContent = 'Pendiente'; chip.className = 'chip pendiente';
  const entregar = () => { chip.textContent = 'Entregado'; chip.className = 'chip entregado'; if (!reducir) gsap.from(chip, { scale: 0.6, duration: 0.4, ease: 'back.out(2)' }); };
  if (reducir) { entregar(); return; }
  gsap.fromTo(slide.querySelector('.lenta'), { width: 0 }, { width: '100%', duration: 6, ease: 'none' });
  gsap.fromTo(slide.querySelector('.rapida'), { width: 0 }, { width: '7.1%', duration: 6 * (30 / 420), ease: 'none' });
  girarVisor(slide.querySelector('.flap'));
  gsap.delayedCall(1.8, entregar);
}

/* ---------- barras de metas ---------- */
function barras(slide) {
  if (reducir) return;
  slide.querySelectorAll('.kpi-barra').forEach((b, i) => {
    gsap.fromTo(b, { width: 0 }, { width: b.style.getPropertyValue('--v'), duration: 1.1, delay: 0.15 * i, ease: 'power2.out', clearProps: 'width' });
  });
}

/* ---------- tarjetas que se dan vuelta ---------- */
document.querySelectorAll('.flip').forEach((f) => {
  f.addEventListener('click', () => f.setAttribute('aria-pressed', f.getAttribute('aria-pressed') === 'true' ? 'false' : 'true'));
});


/* ---------- dibujo: la fila avanza de a uno mientras corre el recreo ---------- */
function fila(slide) {
  const alumnos = [...slide.querySelectorAll('.alumno')];
  const sector = slide.querySelector('.reloj-sector');
  const aguja = slide.querySelector('.reloj-aguja');
  const X0 = 120, PASO = 110;
  alumnos.forEach((a, i) => { a.removeAttribute('transform'); gsap.set(a, { x: X0 + PASO * i, opacity: 1 }); });
  const pintarReloj = (frac) => {
    const ang = frac * Math.PI * 2 - Math.PI / 2;
    const x = 30 * Math.cos(ang), y = 30 * Math.sin(ang);
    sector.setAttribute('d', frac <= 0 ? 'M0,0 Z' : `M0,0 L0,-30 A30,30 0 ${frac > 0.5 ? 1 : 0} 1 ${x.toFixed(2)},${y.toFixed(2)} Z`);
    aguja.setAttribute('x2', (26 * Math.cos(ang)).toFixed(2));
    aguja.setAttribute('y2', (26 * Math.sin(ang)).toFixed(2));
  };
  if (reducir) { pintarReloj(0.7); return; }
  const reloj = { f: 0 };
  const tReloj = gsap.to(reloj, { f: 1, duration: 16, ease: 'none', repeat: -1, onUpdate: () => pintarReloj(reloj.f) });
  let orden = [...alumnos];
  const ciclo = () => {
    const frente = orden[orden.length - 1];
    gsap.to(frente, { x: 1250, opacity: 0, duration: 0.6, ease: 'power1.in' });
    orden.slice(0, -1).forEach((a) => gsap.to(a, { x: `+=${PASO}`, duration: 0.7, ease: 'power2.inOut', delay: 0.25 }));
    gsap.set(frente, { x: X0 - PASO, delay: 0.7 });
    gsap.to(frente, { x: X0, opacity: 1, duration: 0.6, delay: 0.75 });
    orden = [frente, ...orden.slice(0, -1)];
  };
  // 6 vueltas (unos 10 s) y se detiene: sin animaciones infinitas de fondo
  let vueltas = 0;
  const t = setInterval(() => { ciclo(); if (++vueltas >= 6) { clearInterval(t); tReloj.pause(); } }, 1600);
  slide._parar = () => { clearInterval(t); tReloj.kill(); };
}

/* ---------- dibujo: un pedido viaja por la base ---------- */
function flujo(slide) {
  const paquete = slide.querySelector('.paquete');
  const texto = slide.querySelector('#flujo-texto');
  const tablas = [...slide.querySelectorAll('.tabla')];
  const estado = slide.querySelector('.estado-retiro');
  const aristas = ['#r1', '#r2', '#r3', '#r4'].map((id) => slide.querySelector(id));
  const PASOS = [
    { t: 'Al confirmar, se crea la Venta y se descuenta el stock del Producto.', tablas: [0, 1] },
    { t: 'La base genera el Retiro con su código único y se lo devuelve al alumno.', tablas: [2] },
    { t: 'En el recreo, el cajero escribe el código y la base trae el pedido.', tablas: [2] },
    { t: 'Al entregar, el Retiro pasa a ENTREGADO y el código no se puede volver a usar.', tablas: [2], fin: true },
  ];
  const marcar = (i) => {
    aristas.forEach((a, j) => a.classList.toggle('activa', i === j));
    tablas.forEach((tb, j) => tb.classList.toggle('activa', i >= 0 && PASOS[i].tablas.includes(j)));
    if (i >= 0) texto.textContent = PASOS[i].t;
    estado.textContent = i === 3 ? 'ENTREGADO' : 'PENDIENTE';
    estado.classList.toggle('ok', i === 3);
  };
  if (reducir) { marcar(3); gsap.set(paquete, { opacity: 0 }); return; }
  const tl = gsap.timeline({ repeat: 1, repeatDelay: 1.6, onRepeat: () => marcar(-1) });
  aristas.forEach((a, i) => {
    tl.call(() => marcar(i));
    tl.to(paquete, { duration: 1.3, ease: 'power1.inOut', motionPath: { path: a, align: a, alignOrigin: [0.5, 0.5] } });
    tl.to({}, { duration: 1.4 });
  });
  slide._parar = () => { tl.kill(); marcar(-1); };
}

/* ---------- portada: fila generativa (semilla fija, inspirada en algorithmic-art) ----------
   Cada punto es un alumno. Entran por la izquierda, serpentean como una fila
   y desembocan en el visor del código, donde desaparecen: el pedido se retiró. */
function filaGenerativa(slide) {
  const cv = slide.querySelector('.fila-generativa');
  if (!cv) return;
  const ctx = cv.getContext('2d');
  const W = 1920, H = 1080, dpr = Math.min(window.devicePixelRatio || 1, 2);
  cv.width = W * dpr; cv.height = H * dpr; ctx.scale(dpr, dpr);
  let semilla = 0x3c6b; // "3C6MB"
  const azar = () => { semilla = (semilla + 0x6d2b79f5) | 0; let t = Math.imul(semilla ^ (semilla >>> 15), 1 | semilla); t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t; return ((t ^ (t >>> 14)) >>> 0) / 4294967296; };
  const camino = (u) => {
    // la fila entra por abajo (lejos del título), serpentea y sube hasta debajo del visor
    const x = -60 + u * 1620;
    const s = Math.min(1, Math.max(0, (u - 0.5) / 0.5));
    const subida = s * s * (3 - 2 * s);
    const amp = 46 * (1 - u) + 8;
    const y = 1010 - subida * 370 + amp * Math.sin(u * Math.PI * 7);
    return [x, y];
  };
  const N = 220;
  const ps = Array.from({ length: N }, () => ({ u: azar(), v: (azar() - 0.5) * 2, vel: 0.0007 + azar() * 0.0011, r: 2 + azar() * 3.2, fase: azar() * 6.28 }));
  const dibujar = (tiempo) => {
    ctx.fillStyle = 'rgba(46,21,27,0.28)';
    ctx.fillRect(0, 0, W, H);
    for (const p of ps) {
      const [x, y] = camino(p.u);
      const ancho = 26 * (1 - p.u) + 5;
      const oy = p.v * ancho + Math.sin(tiempo * 0.001 + p.fase) * 4;
      const alfa = Math.min(1, p.u * 8) * (1 - Math.max(0, (p.u - 0.86) / 0.14));
      ctx.beginPath();
      ctx.fillStyle = `rgba(229,184,66,${(0.18 + 0.5 * alfa).toFixed(3)})`;
      ctx.arc(x, y + oy, p.r * (0.6 + 0.4 * alfa), 0, Math.PI * 2);
      ctx.fill();
    }
  };
  if (reducir) { ctx.fillStyle = '#2e151b'; ctx.fillRect(0, 0, W, H); for (let k = 0; k < 40; k++) dibujar(k * 16); return; }
  let vivo = true;
  const inicio = performance.now();
  const paso = (tiempo) => {
    if (!vivo || tiempo - inicio > 14000) return; // se detiene a los 14 s
    for (const p of ps) { p.u += p.vel; if (p.u > 1) { p.u = 0; p.v = (azar() - 0.5) * 2; } }
    dibujar(tiempo);
    requestAnimationFrame(paso);
  };
  ctx.fillStyle = '#2e151b'; ctx.fillRect(0, 0, W, H);
  requestAnimationFrame(paso);
  slide._parar = () => { vivo = false; };
}

/* ---------- entradas por diapositiva ---------- */
const ENTRADAS = {
  portada: (s) => { girarVisor(s.querySelector('.flap')); filaGenerativa(s); },
  fila: (s) => { contar(s); fila(s); },
  flujo,
  contar,
  carrera,
  barras,
};

/* ---------- reloj de recreo: 10:35 a 10:45 a lo largo de la presentación ---------- */
const hora = document.getElementById('recreo-hora');
const avance = document.getElementById('recreo-avance');
const seccion = document.getElementById('recreo-seccion');

function actualizarRecreo(slide) {
  const total = Reveal.getTotalSlides();
  const i = Reveal.getSlidePastCount();
  const frac = total > 1 ? i / (total - 1) : 0;
  const minutos = 35 + Math.round(frac * 10);
  hora.textContent = `10:${String(minutos).padStart(2, '0')}`;
  avance.style.width = `${frac * 100}%`;
  seccion.textContent = slide.dataset.seccion || '';
  document.body.classList.toggle('tono-oscuro', slide.dataset.tono === 'oscuro');
}

const deck = new Reveal({
  width: 1920,
  height: 1080,
  margin: 0,
  minScale: 0.1,
  maxScale: 2,
  center: false,
  hash: false,
  controls: true,
  controlsTutorial: false,
  progress: false,
  slideNumber: false,
  transition: 'fade',
  transitionSpeed: 'fast',
  plugins: [Notes],
});

// Reveal expone la instancia también como singleton para getTotalSlides, etc.
Object.assign(Reveal, { getTotalSlides: () => deck.getTotalSlides(), getSlidePastCount: () => deck.getSlidePastCount() });

deck.initialize().then(() => {
  const inicial = deck.getCurrentSlide();
  actualizarRecreo(inicial);
  ENTRADAS[inicial.dataset.entrada]?.(inicial);
});

deck.on('slidechanged', ({ currentSlide, previousSlide }) => {
  previousSlide?._parar?.();
  actualizarRecreo(currentSlide);
  ENTRADAS[currentSlide.dataset.entrada]?.(currentSlide);
});
