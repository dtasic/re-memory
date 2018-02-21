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

type action = 
  | OpenTile(Data.tile)
  | CheckTiles(Data.tile)
  | CloseTiles
  | ResetGame;

/* main App */

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  initialState: () => {
    tiles: Utils.shuffleTiles(Data.rawtiles),
    openedtile: None,
    closing: false,
    attempts: 0,
    finished: false
  },
  reducer: (action, state) =>
    switch action {
    | ResetGame => 
      let tiles = List.map((tile: Data.tile) => {...tile, visible: false, solved: false }, state.tiles);
      ReasonReact.Update({...state, closing: false, attempts: 0, finished: false, tiles: Utils.shuffleTiles(tiles)})
    | CloseTiles => 
      let tiles = List.map((tile: Data.tile) => {...tile, visible: false }, state.tiles);
      ReasonReact.Update({ tiles: tiles, closing: false, finished: Utils.isAllSolved(tiles), attempts: (state.attempts + 1), openedtile: None })
    | CheckTiles(currenttile) => 
      if (Utils.getOpenedTileSport(state.openedtile) === currenttile.sport) {
        let tiles = 
          List.map(
            (tile: Data.tile) => 
            tile.id === Utils.getOpenedTileId(state.openedtile) || tile.id === currenttile.id ? 
              {...tile, solved: true } : tile,
            state.tiles
          );
        ReasonReact.Update({ tiles: tiles, closing: false, finished: Utils.isAllSolved(tiles), attempts: (state.attempts + 1), openedtile: None });
      } else {
        ReasonReact.SideEffects(self => ignore(Js.Global.setTimeout(() => self.send(CloseTiles), Utils.time_delay)));
      };
    | OpenTile(currenttile) => 
      let tiles =
        List.map(
          (tile: Data.tile) =>
          tile.id === currenttile.id ?
              {...tile, visible: true } : tile,
          state.tiles
        );
      if (Utils.getOpenedTileId(state.openedtile) !== 0) {
        ReasonReact.UpdateWithSideEffects({ ...state, tiles: tiles, closing: true }, (self) => self.send(CheckTiles(currenttile)));
      } else {
        ReasonReact.Update({ ...state, tiles: tiles, openedtile: Some(currenttile) })
      }
      
    },
  render: (self) => {
    let finished = 
      switch (self.state.finished) {
      | true => (Utils.str("Game is finished!"))
      | false => (Utils.str(""))
      };
    <div className="app">
    <div className="header">
        <span className="left">(Utils.str("Re-memory - Reason-React memory game"))</span>
        <span className="right">(finished)</span>
      </div>
      <div className="tiles">
        (
          ReasonReact.arrayToElement(Array.of_list(List.map(
            (tile: Data.tile) =>
            <Tile
                key=(string_of_int(tile.id))
                closing=(self.state.closing)
                onClick=(_e => self.send(OpenTile(tile)))
                tile
              />,
              self.state.tiles
          )))
        )
      </div>
      <div className="footer">
        <span className="left">(Utils.str("Attempts: " ++ string_of_int(self.state.attempts)))</span>
        <span className="right">
          <button onClick=(_e => self.send(ResetGame))>(Utils.str("reset"))</button>
        </span>
      </div>
    </div>
  }
};
