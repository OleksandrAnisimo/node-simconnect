export enum SimConnectException {
    NONE,
    ERROR,
    SIZE_MISMATCH,
    UNRECOGNIZED_ID,
    UNOPENED,
    VERSION_MISMATCH,
    TOO_MANY_GROUPS,
    NAME_UNRECOGNIZED,
    TOO_MANY_EVENT_NAMES,
    EVENT_ID_DUPLICATE,
    TOO_MANY_MAPS,
    TOO_MANY_OBJECTS,
    TOO_MANY_REQUESTS,
    WEATHER_INVALID_PORT,
    WEATHER_INVALID_METAR,
    WEATHER_UNABLE_TO_GET_OBSERVATION,
    WEATHER_UNABLE_TO_CREATE_STATION,
    WEATHER_UNABLE_TO_REMOVE_STATION,
    INVALID_DATA_TYPE,
    INVALID_DATA_SIZE,
    DATA_ERROR,
    INVALID_ARRAY,
    CREATE_OBJECT_FAILED,
    LOAD_FLIGHTPLAN_FAILED,
    OPERATION_INVALID_FOR_OJBECT_TYPE,
    ILLEGAL_OPERATION,
    ALREADY_SUBSCRIBED,
    INVALID_ENUM,
    DEFINITION_ERROR,
    DUPLICATE_ID,
    DATUM_ID,
    OUT_OF_BOUNDS,
    /** @since 0.5 */
    ALREADY_CREATED,
    /** @since 0.5 */
    OBJECT_OUTSIDE_REALITY_BUBBLE,
    /** @since 0.5 */
    OBJECT_CONTAINER,
    /** @since 0.5 */
    OBJECT_AI,
    /** @since 0.5 */
    OBJECT_ATC,
    /** @since 0.5 */
    OBJECT_SCHEDULE,
}

module.exports = {
    SimConnectException,
};
