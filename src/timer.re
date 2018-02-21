type state = {
    timerWidth: int
};

type action = 
    | UpdateTimer;

let component = ReasonReact.reducerComponent("Timer");

let make = (_children) => {
  ...component,
  initialState: () => {
    timerWidth: 0
  },
  subscriptions: (self) => [
    Sub(
      () => Js.Global.setInterval(() => self.send(UpdateTimer), Utils.timer_update),
      Js.Global.clearInterval
    )
  ],
  reducer: (action, state) =>
    switch action {
    | UpdateTimer =>
        ReasonReact.Update({ timerWidth: state.timerWidth + 1 });
    },
  render: self => {
    <div className="timer" style=(ReactDOMRe.Style.make(~width=string_of_int(Utils.time_delay/Utils.timer_update - self.state.timerWidth) ++ "px", ())) />
  }
}