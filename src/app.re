/* get styles file */

[%bs.raw {|require('./app.css')|}];

/* tiles, state and actions */
type tiles = list(Data.tile);

type state = {
  tiles: tiles,
  openedtile: option(Data.tile),
  closing: bool,
  attempts: int,
  finished: bool
};

let initState = {
  tiles: Utils.shuffleTiles(Data.rawtiles),
  openedtile: None,
  closing: false,
  attempts: 0,
  finished: false
};

let finishAttempt = (tiles, attempts) => 
  {
    tiles,
    closing: false,
    finished: Utils.isAllSolved(tiles),
    attempts: attempts + 1,
    openedtile: None
  };

type action =
  | OpenTile(Data.tile)
  | CheckTiles(Data.tile)
  | CloseWithDelay
  | ResetGame;

/* main App */

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => initState,
  reducer: (action, state) =>
    switch action {
    | ResetGame =>
      ReasonReact.Update({...initState, tiles: Utils.shuffleTiles(Data.rawtiles)});
    | CloseWithDelay =>
      let tiles =
        List.map((tile: Data.tile) => {...tile, status: tile.status !== Solved ? Hidden : Solved}, state.tiles);
      ReasonReact.Update(finishAttempt(tiles, state.attempts));
    | CheckTiles(currenttile) =>
      if (Utils.getOpenedTileSport(state.openedtile) === currenttile.sport) {
        let tiles =
          List.map(
            (tile: Data.tile) =>
              tile.id === Utils.getOpenedTileId(state.openedtile)
              || tile.id === currenttile.id ?
                {...tile, status: Solved} : tile,
            state.tiles
          );
        ReasonReact.Update(finishAttempt(tiles, state.attempts));
      } else {
        ReasonReact.SideEffects(
          self => ignore(Js.Global.setTimeout(() => self.send(CloseWithDelay), Utils.time_delay))
        );
      }
    | OpenTile(currenttile) =>
      let tiles =
        List.map((tile: Data.tile) => tile.id === currenttile.id ? {...tile, status: Visible} : tile,state.tiles);
      if (state.openedtile !== None) {
        ReasonReact.UpdateWithSideEffects(
          {...state, tiles, closing: true}, (self => self.send(CheckTiles(currenttile)))
        );
      } else {
        ReasonReact.Update({...state, tiles, openedtile: Some(currenttile)});
      };
    },
  render: self => {
    let finished =
      self.state.finished ? Utils.str("Game is finished!") : Utils.str("");
    let timer = self.state.closing ? <Timer /> : Utils.str("");
    <div className="app">
      <div className="header">
        <span className="left">
          (Utils.str("Re-memory - Reason-React memory game"))
        </span>
        timer
        <span className="right"> finished </span>
      </div>
      <div className="tiles">
        (
          ReasonReact.arrayToElement(
            Array.of_list(
              List.map(
                (tile: Data.tile) =>
                  <Tile
                    key=(string_of_int(tile.id))
                    closing=self.state.closing
                    onClick=(_e => self.send(OpenTile(tile)))
                    tile
                  />,
                self.state.tiles
              )
            )
          )
        )
      </div>
      <div className="footer">
        <span className="left">
          (Utils.str("Attempts: " ++ string_of_int(self.state.attempts)))
        </span>
        <span className="right">
          <button onClick=(_e => self.send(ResetGame))>
            (Utils.str("reset"))
          </button>
        </span>
      </div>
    </div>;
  }
};