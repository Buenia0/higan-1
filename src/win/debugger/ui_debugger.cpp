bool DebugWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    debugger.deactivate();
    return true;
  } break;

  case EVENT_CLICKED: {
    if(info.control == &Memory) {
      wMemory.Show();
    } else if(info.control == &Run) {
      if(bsnes->get_state() == bSNES::RUN) {
        bsnes->set_state(bSNES::STOP);
      } else if(bsnes->get_state() == bSNES::STOP) {
        bsnes->set_state(bSNES::RUN);
      }
    }
  } break;

  }

  return false;
}

void DebugWindow::Update() {
  if(Visible() == false)return;

char t[250 * 132];
  sprintf(t, "");
  for(uint i = 0; i < buffer.count; i++) {
    strcat(t, buffer.line[i]);
    if(i != buffer.count - 1)strcat(t, "\r\n");
  }
  Console.SetText(t);
  Console.SetSelection(0, -1); //scroll cursor to bottom of window
}

void DebugWindow::Print(const char *str) {
  if(!hwnd)return;

  for(uint i = 0; i < buffer.count - 1; i++) {
    strcpy(buffer.line[i], buffer.line[i + 1]);
  }

  strcpy(buffer.line[buffer.count - 1], str);
char *t = buffer.line[buffer.count - 1];
  if(strlen(t) > 80) {
    t[77] = '.';
    t[78] = '.';
    t[79] = '.';
    t[80] = 0;
  }
  Update();
}

void DebugWindow::Clear() {
  for(uint i = 0; i < buffer.count; i++) {
    strcpy(buffer.line[i], "");
  }
  Console.SetText("");
}

//called by bSNES::set_state() to update debug UI
void DebugWindow::SetState(uint state) {
  if(Visible() == false)return;

  switch(state) {
  case bSNES::RUN:  Run.SetText(" Stop"); break;
  case bSNES::STOP: Run.SetText(" Run");  break;
  }
}

void DebugWindow::Show() {
  Run.SetText((bsnes->get_state() == bSNES::RUN) ? " Stop" : " Run");
  Window::Show();
  Update();
}

void DebugWindow::Setup() {
int x = 5, y = 5;
  Breakpoints.Create(this, "visible|left", x, y, 80, 20, " Breakpoints");
  x += 80;
  Memory.Create(this, "visible|left", x, y, 80, 20, " Memory");
  x += 80;

  x = 680;
  RunToFrame.Create(this, "visible|left", x, y, 80, 20, " Run Frame");
  x += 80;
  Run.Create(this, "visible|left", x, y, 80, 20, " Run");
  x = 5;
  y += 25;

  Separator.Create(this, "visible|sunken", x, y, 840, 3);
  y += 8;

  Console.Create(this, "visible|edge|multiline|vscroll|readonly", x, y, 585, 290);
  Console.SetFont(global::fwf);
  Console.SetBackgroundColor(32, 32, 32);
  Console.SetTextColor(255, 255, 255);

  Status.Create(this, "visible|edge|multiline|readonly", x, y + 290, 585, 52);
  Status.SetFont(global::fwf);
  Status.SetBackgroundColor(32, 32, 32);
  Status.SetTextColor(255, 255, 255);
  x += 590;

  CPUGroup.Create(this, "visible", x, y, 250, 60, "CPU");
  CPUStep.Create(this, "visible|left", x + 5, y + 15, 80, 20, " Step");
  CPUProceed.Create(this, "visible|left", x + 85, y + 15, 80, 20, " Proceed");
  CPUSkip.Create(this, "visible|left", x + 165, y + 15, 80, 20, " Skip");
  CPUTraceNum.Create(this, "visible|edge", x + 5, y + 35, 80, 20, "0");
  CPUTraceNum.SetFont(global::fwf);
  CPUTrace.Create(this, "visible|left", x + 85, y + 35, 80, 20, " Trace");
  CPUDisable.Create(this, "visible|left", x + 165, y + 35, 80, 20, " Disable");
  y += 66;

  APUGroup.Create(this, "visible", x, y, 250, 60, "APU");
  APUStep.Create(this, "visible|left", x + 5, y + 15, 80, 20, " Step");
  APUProceed.Create(this, "visible|left", x + 85, y + 15, 80, 20, " Proceed");
  APUSkip.Create(this, "visible|left", x + 165, y + 15, 80, 20, " Skip");
  APUTraceNum.Create(this, "visible|edge", x + 5, y + 35, 80, 20, "0");
  APUTraceNum.SetFont(global::fwf);
  APUTrace.Create(this, "visible|left", x + 85, y + 35, 80, 20, " Trace");
  APUDisable.Create(this, "visible|left", x + 165, y + 35, 80, 20, " Disable");
  y += 66;

  buffer.count = uint(config::debugger.console_lines);
  buffer.count = (buffer.count < 20) ? 20 : (buffer.count > 250) ? 250 : buffer.count;
  Clear();
}