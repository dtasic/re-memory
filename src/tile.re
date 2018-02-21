/* single Tile */

let component = ReasonReact.statelessComponent("Tile");
let make = (~tile: Data.tile, ~closing, ~onClick, _children) => {
  ...component,
  render: (_self) =>
    <div className=(
        "tile " 
        ++ (tile.visible || tile.solved ? tile.sport : "") 
        ++ (tile.visible ? " visible" : "") 
        ++ (tile.solved ? " solved" : "")
      ) 
      onClick=(_e => (!tile.solved && !tile.visible && !closing) ? onClick(tile) : ()
    )>
      (Utils.str(tile.visible || tile.solved ? "" : "?"))
    </div>
};
