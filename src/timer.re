/* closing tiles timer (visual countdown) */

let component = ReasonReact.reducerComponent("Timer");

let make = (_children) => {
  ...component,
  initialState: () => 0,
  subscriptions: (self) => [
    Sub(
      () => Js.Global.setInterval(() => self.send(), Utils.timer_update),
      Js.Global.clearInterval
    )
  ],
  reducer: (_action, state) => ReasonReact.Update({ state + 1 }),
  render: self => {
    <div className="timer" style=(ReactDOMRe.Style.make(~width=string_of_int(Utils.time_delay/Utils.timer_update - self.state) ++ "px", ())) />
  }
}